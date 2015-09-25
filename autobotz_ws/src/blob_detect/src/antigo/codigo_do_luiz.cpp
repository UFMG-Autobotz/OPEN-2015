#include <cmath>
#include <cv.h> 
#include <cv_bridge/cv_bridge.h>
#include <cxcore.h>
#include "findContours.hpp"
#include <highgui.h>
#include <image_transport/image_transport.h>	
#include <iostream> 
#include <opencv/cv.h>
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>	
#include <opencv2/imgproc/imgproc.hpp>	
#include <opencv2/opencv.hpp>
#include <ros/ros.h>	
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <sstream>
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
//#include "stitcher/Num.h"
#include <string.h> 
#include <vector>
#include <X11/keysym.h>

using namespace cv;
using namespace std;

image_transport::Publisher pub; 

int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";


///////////////////////////////////// declaracoes ///////////////////////////////////////////

Point RefPointYellow;
Point RefPointYellowDepth;

Mat binary, image_rgb, image_rgb_previous, image_merge, image_yellow, image_red; 
Mat Rchannel, Gchannel, Bchannel; // Variáveis que guardarão cada um dos 3 canais da imagem HSV

vector<vector<Point> > squaresYellow; // Vetor de pontos para os Blocos Amarelos
vector<vector<Point> > squaresRed; // Vetor de pontos para os Blocos Vermelhos

typedef union U_FloatParse{
	    float float_data;
	    unsigned char byte_data[4];
}U_FloatConvert;

////////////////////////////////////// leitura de profundidade //////////////////////////////////////////////

// Método responsável por fornecer a distância em mm de um ponto fornecido com parâmetro
int ReadDepthData(unsigned int height_pos, unsigned int width_pos, sensor_msgs::ImageConstPtr image){
	// Checa se as coordenadas do ponto passado como parâmetro são válidas	
	if((height_pos > image->height) || (width_pos > image->width))
        	return -1;
        int index = (height_pos*image->step) + (width_pos*(image->step/image->width));
        // If data is 4 byte floats (rectified depth image)
        if((image->step/image->width) == 4){
        	U_FloatConvert depth_data;
        	int i, endian_check = 1;
        	// If big endian
        	if((image->is_bigendian && (*(char*)&endian_check != 1))|| ((!image->is_bigendian) && (*(char*)&endian_check == 1))){ 
            		for(i = 0; i < 4; i++)
                		depth_data.byte_data[i] = image->data[index + i];
            		// Make sure data is valid (check if NaN)
            		if(depth_data.float_data == depth_data.float_data)
                		return int(depth_data.float_data*1000);
            		return -1;  // If depth data invalid
        	}
        	// else, one little endian, one big endian
        	for(i = 0; i < 4; i++) 
            		depth_data.byte_data[i] = image->data[3 + index - i];
        	// Make sure data is valid (check if NaN)
        	if (depth_data.float_data == depth_data.float_data)
            		return int(depth_data.float_data*1000);
        	return -1;  // If depth data invalid
    	}
    	// Otherwise, data is 2 byte integers (raw depth image)
   	int temp_val;
   	// If big endian
   	if(image->is_bigendian)
       		temp_val = (image->data[index] << 8) + image->data[index + 1];
   	// If little endian
   	else
       		temp_val = image->data[index] + (image->data[index + 1] << 8);
   	// Make sure data is valid (check if NaN)
   	if(temp_val == temp_val)
       		return temp_val;
   	return -1;  // If depth data invalid
}

////////////////////////////////////// trackbar //////////////////////////////////////////////

// Declaração e inicialização dos valores HSV do Trackbar para os Blocos Amarelos
int H_yellow_min = 0, H_yellow_max = 152, 
    S_yellow_min = 185, S_yellow_max = 255,
    V_yellow_min = 118, V_yellow_max = 238; 

// Declaração e inicialização dos valores HSV do Trackbar para os Blocos Vermelhos
int H_red_min = 0, H_red_max = 255, 
    S_red_min = 135, S_red_max = 255,
    V_red_min = 164, V_red_max = 255; 


// Método que cria Trackbar de 0 a 255 para ajuste de valores de HSV pretendidos para o Bloco Amarelo
void creatTrackbarYellow(){
	namedWindow("HSV Yellow Adjustment", CV_WINDOW_AUTOSIZE); 
	cvCreateTrackbar("H_yellow_min", "HSV Yellow Adjustment", &H_yellow_min, 255); 
  	cvCreateTrackbar("H_yellow_max", "HSV Yellow Adjustment", &H_yellow_max, 255); 
  	cvCreateTrackbar("S_yellow_min", "HSV Yellow Adjustment", &S_yellow_min, 255); 
  	cvCreateTrackbar("S_yellow_max", "HSV Yellow Adjustment", &S_yellow_max, 255); 
  	cvCreateTrackbar("V_yellow_min", "HSV Yellow Adjustment", &V_yellow_min, 255); 
  	cvCreateTrackbar("V_yellow_max", "HSV Yellow Adjustment", &V_yellow_max, 255); 
}

// Método que cria Trackbar de 0 a 255 para ajuste de valores de HSV pretendidos para o Bloco Vermelho
void creatTrackbarRed(){
	namedWindow("HSV Red Adjustment", CV_WINDOW_AUTOSIZE); 
	cvCreateTrackbar("H_red_min", "HSV Red Adjustment", &H_red_min, 255); 
  	cvCreateTrackbar("H_red_max", "HSV Red Adjustment", &H_red_max, 255); 
  	cvCreateTrackbar("S_red_min", "HSV Red Adjustment", &S_red_min, 255); 
  	cvCreateTrackbar("S_red_max", "HSV Red Adjustment", &S_red_max, 255); 
  	cvCreateTrackbar("V_red_min", "HSV Red Adjustment", &V_red_min, 255); 
  	cvCreateTrackbar("V_red_max", "HSV Red Adjustment", &V_red_max, 255); 
}

//////////////////////////////// filtro preliminar?   ////////////////////////////////////////////

// Método responsável por aplicar transformações morfológicas nos canais de uma imagem
void morphOps(Mat &thresh){
 	Mat erodeElement = getStructuringElement(MORPH_RECT,Size(3,3));  // Definição da estrutura do elemento a ser erodido	
 	Mat dilateElement = getStructuringElement(MORPH_RECT,Size(8,8));  // Definição da estrutura do elemento a ser dilatado
        erode(thresh,thresh,erodeElement);
        dilate(thresh,thresh,dilateElement);
}

// Método que separa a imagem HSV em 3 canais, usa transformações morfológicas em cada um deles, e depois junta os 3 canais novamente.
void filter(){
	std::vector<cv::Mat> three_channels; // Cria um vetor para comportar cada os 3 canais
	split(image_rgb, three_channels);  // Divide a imagem RGB em 3 canais
 	Rchannel = three_channels[0];	// Alocação de cada canal em uma posição do vetor criado
	Gchannel = three_channels[1];
	Bchannel = three_channels[2];
	morphOps(Rchannel);  // Transformação morfológica em cada um dos canais
	morphOps(Gchannel);
	morphOps(Bchannel);	
	merge(three_channels, image_merge);  // Junta os 3 canais em uma só imagem
	//namedWindow("Merge Image"); 
	//imshow("Merge Image", image_merge); 
	namedWindow("Original Image"); 
	imshow("Original Image", image_rgb); 
}

// Método auxiliar que encontra o cosseno do ãngulo entre dois vetores
static double angle(Point pt1, Point pt2, Point pt0)
{
    	double dx1 = pt1.x - pt0.x;
    	double dy1 = pt1.y - pt0.y;
    	double dx2 = pt2.x - pt0.x;
    	double dy2 = pt2.y - pt0.y;
    	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

///////////////////////////////////////// contorno() /////////////////////////////////////////////////
void Contorno()
{
 	// IplImage* img = new IplImage(image_rgb);

	// Converte a imagem original para "grayscale"
	cv::Mat gray;
	cv::cvtColor(image_rgb, gray, CV_BGR2GRAY);

	// Converte para uma imagem binária utilizando Canny
	cv::Mat bw;
	cv::Canny(gray, bw, 1000, 3000, 5);  // OBS: Esses parâmetros podem ser modificados

	//namedWindow("Canny Image"); 
	//imshow("Canny Image", bw); 
	cv::Mat bw1;
	cv::Canny(gray, bw1, 100, 300, 5);  // OBS: Esses parâmetros podem ser modificados

	//namedWindow("Canny Image2");
	//imshow("Canny Image2", bw1); 

	vector<vector<Point> > contours;
   	vector<Vec4i> hierarchy;
    	RNG rng(12345);
    	findContours(bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    	/// Draw contours
    	Mat drawing = Mat::zeros(gray.size(), CV_8UC3 );
    	for(int i = 0; i< contours.size(); i++){
        	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    	}     

	//namedWindow("Draw Image"); 
	//imshow("Draw Image", drawing); 

}

///////////////////////////////////////////////  encontra quadrados ///////////////////////////////////////////////////

static void findSquares(const Mat& image, vector<vector<Point> >& squares){
    	squares.clear();  // Limpa o vetor de pontos
    	Mat pyr,  // Imagem após o "Down-scale" 
	    timg,   // Imagem após o "Up-scale"
            gray0(image.size(), CV_8U), 
            gray;

    	// "Down-scale" e "Up-scale" a imagem para filtrar o ruído
    	pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    	pyrUp(pyr, timg, image.size());

	// Declaração de um vetor de vetor de pontos
    	vector<vector<Point> > contours;

    	// Encontra retângulos em cada um dos planos de cores da imagem
    	for(int c = 0; c < 3; c++){
        	int ch[] = {c, 0};
        	mixChannels(&timg, 1, &gray0, 1, ch, 1);

        	// Tenta diversos níveis de threshold 
        	for(int l = 0; l < N; l++){
            		if(l == 0){  // Truque: usar Canny ao invés do threshold nível zero
				Canny(gray0, gray, 0, thresh, 5);
				dilate(gray, gray, Mat(), Point(-1,-1));
           		}
        	    	else  // Aplica o método threshold caso l!=0
			        gray = gray0 >= (l+1)*255/N;

        	    	// Encontra os contornos e os guarda em uma lista
        	    	findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

			// Declaração de um vetor de pontos chamado "approx"
			vector<Point> approx;

			// Testa cada contorno
			for(size_t i = 0; i < contours.size(); i++){
				// Aproxima o contorno com acurácia proporcional ao seu perímetro
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
				// Os contornos retângulos devem ter 4 vértices após a aproximação e serem convexos. OBS: Os valores absolutos 					devem ser usados porque a área pode ser positiva ou negativa, dependendo da orientação do contorno
				if(approx.size() == 4 && fabs(contourArea(Mat(approx))) > 100 && isContourConvex(Mat(approx))){
                			double maxCosine = 0;
	                        	for(int j = 2; j < 5; j++){
                       				// Encontra o máximo cosseno do ângulo entre duas juntas
                       				double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                       				maxCosine = MAX(maxCosine, cosine);
                			}
					// Se os cossenos de todos os ângulos são pequenos (todos os ângulos são ~90 graus) então escreve os 						vértices na sequência resultante
					if(maxCosine < 0.26)
                       				squares.push_back(approx);
               			}
			}
        	}
    	}
}

/////////////////////////////////////////// encontra blocos amarelos /////////////////////////////////////////

static void findYellowBlocks(const Mat& image, vector<vector<Point> >& squares){
    	squares.clear();  // Limpa o vetor de pontos

    	Mat gray, image_hsv;

	image_rgb.copyTo(image_yellow);	

    	vector<vector<Point> > contours;
	vector<Point> approx;

	cvtColor(image, image_hsv, CV_BGR2HSV); 

	inRange(image_hsv, Scalar(H_yellow_min, S_yellow_min, V_yellow_min), Scalar(H_yellow_max, S_yellow_max, V_yellow_max), gray);	
	namedWindow("Yellow Adjustment");
        imshow("Yellow Adjustment", gray);

        // Encontra os contornos e os guarda em uma lista
        findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	/*
	for(int l = 0; l < contours.size(); l++)
		for(int m = 0; m < contours.size(); m++)
			ROS_INFO("ID = %i %i\n", contours[l][m].x, contours[l][m].y);*/
	
	// Testa cada contorno
	for(size_t i = 0; i < contours.size(); i++){

		// Aproxima o contorno com acurácia proporcional ao seu perímetro
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		// Os contornos retângulos devem ter 4 vértices após a aproximação e serem convexos. OBS: Os valores absolutos 			devem ser usados porque a área pode ser positiva ou negativa, dependendo da orientação do contorno
		if(approx.size() == 4 && fabs(contourArea(Mat(approx))) > 100 && isContourConvex(Mat(approx))){
        		double maxCosine = 0;
	               	for(int j = 2; j < 5; j++){
        			// Encontra o máximo cosseno do ângulo entre duas juntas
                       		double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                       		maxCosine = MAX(maxCosine, cosine);
                	}
			// Se os cossenos de todos os ângulos são pequenos (todos os ângulos são ~90 graus) então escreve os 				vértices na sequência resultante
			if(maxCosine < 0.26){
                       		squares.push_back(approx);
				/*for(int c = 0; c < approx.size(); c++)
					ROS_INFO("ID = %i %i\n", approx[c].x, approx[c].y);*/
				
				RefPointYellow.x = 0;
				RefPointYellow.y = 0;
				for(int g = 1; g <= approx.size(); g++){
					RefPointYellow.x += approx[g].x; 
					RefPointYellow.y += approx[g].y;  
					if(g%4 == 0){
						RefPointYellowDepth.x = RefPointYellow.x/4;
						RefPointYellowDepth.y = RefPointYellow.y/4;   			
						//ROS_INFO("ID = %i %i\n", RefPointYellowDepth.x, RefPointYellowDepth.y);
						RefPointYellow.x = 0;
						RefPointYellow.y = 0;
					}
				}
			}
               	}
	}
}

static void findRedBlocks(const Mat& image, vector<vector<Point> >& squares){
    	squares.clear();  // Limpa o vetor de pontos

    	Mat gray, image_hsv;

	image_rgb.copyTo(image_red);	

    	vector<vector<Point> > contours;

	cvtColor(image, image_hsv, CV_BGR2HSV); 

	inRange(image_hsv, Scalar(H_red_min, S_red_min, V_red_min), Scalar(H_red_max, S_red_max, V_red_max), gray);	
	namedWindow("Red Adjustment");
        imshow("Red Adjustment", gray);

        	    	// Encontra os contornos e os guarda em uma lista
        	    	findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	
			vector<Point> approx;

			// test each contour
			for(size_t i = 0; i < contours.size(); i++){
				// approximate contour with accuracy proportional
				// to the contour perimeter
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				if(approx.size() == 4 && fabs(contourArea(Mat(approx))) > 100 && isContourConvex(Mat(approx))){
                			double maxCosine = 0;
	                        	for(int j = 2; j < 5; j++){
                       				// Encontra o cosseno máximo do ângulo entre bordas de junção
                       				double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
						ROS_INFO("tutorialROSOpenCV::main.cpp::No error.");
                       				maxCosine = MAX(maxCosine, cosine);
                			}
					// Caso os cossenos do ângulo seja pequeno (~90 graus) então escreve os pontos na sequência
					if(maxCosine < 0.26){
                       				squares.push_back(approx);
					}
               			}
			}
}

// Método que desenha os quadrados na imagem
static void drawSquares(Mat& image, const vector<vector<Point> >& squares){
    	for(size_t i = 0; i < squares.size(); i++){
        	const Point* p = &squares[i][0];
        	int n = (int)squares[i].size();
        	polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
    	}
}

int auxiliar = 0;   ///  ?????????


//////////////////////////////////////// desenha pontos na tela //////////////////////////////////////////

void draw(){  //simplesmente desenha quatro pontos na tela
	
	cv::Point coor1(240, 180);
	string coord1 = ".";
	cv::Point coor2(280, 180);
	string coord2 = ".";
	cv::Point coor3(240, 140);
	string coord3 = ".";
	cv::Point coor4(280, 140);
	string coord4 = ".";
	putText(image_yellow, coord1, coor1, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord2, coor2, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord3, coor3, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord4, coor4, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	
	/*cv::Point coor1(80, 60);
	string coord1 = "80,60";
	cv::Point coor2(160, 120);
	string coord2 = "160,120";
	cv::Point coor3(240,180);
	string coord3 = "240,180";
	cv::Point coor4(320, 240);
	string coord4 = "320,240";
	cv::Point coor5(400, 300);
	string coord5 = "400,300";
	cv::Point coor6(480, 360);
	string coord6 = "480,360";
	cv::Point coor7(560, 420);
	string coord7 = "560,420";
	putText(image_yellow, coord1, coor1, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord2, coor2, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord3, coor3, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord4, coor4, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord5, coor5, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord6, coor6, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
	putText(image_yellow, coord7, coor7, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);*/
}

int aux = 0,  // Variável que conta quantas imagens foram capturadas da câmera 
    depth = 0;

/////////////////////////////////////////////////////// fluxo ótico  /////////////////////////////////////////////////

void opticalFlow(Mat imgA, Mat imgB){
	const int MAX_CORNERS = 500;
	IplImage* imgC = new IplImage(imgB);
	Mat imgA1, imgB1;
	cv::cvtColor(imgA, imgA1, CV_BGR2GRAY);
	cv::cvtColor(imgB, imgB1, CV_BGR2GRAY);
	IplImage* previous = new IplImage(imgA1);
	IplImage* next = new IplImage(imgB1);
	CvSize img_sz = cvGetSize(previous);
	int win_size = 10;
	// The first thing we need to do is get the features we want to track.
	IplImage* eig_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1 );
	IplImage* tmp_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1 );
	int corner_count = MAX_CORNERS;
	CvPoint2D32f* cornersA = new CvPoint2D32f[MAX_CORNERS];
	cvGoodFeaturesToTrack(previous, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);
	cvFindCornerSubPix(previous, cornersA, corner_count, cvSize(win_size,win_size), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_ITER|	CV_TERMCRIT_EPS,20,0.03));
	// Call the Lucas Kanade algorithm
	 char features_found[MAX_CORNERS];
	 float feature_errors[MAX_CORNERS];
	 CvSize pyr_sz = cvSize(previous->width+8, next->height/3);
	 IplImage* pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	 IplImage* pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	 CvPoint2D32f* cornersB = new CvPoint2D32f[MAX_CORNERS];
	 cvCalcOpticalFlowPyrLK(previous, next, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, features_found,
feature_errors, cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3), 0);
	 // Now make some image of what we are looking at:
	 for(int i=0; i<corner_count; i++){
	 	if(features_found[i]==0 || feature_errors[i]>550){
	 		ROS_INFO("Error is %f/n", feature_errors[i]);
	 		continue;
	 	}
	 	ROS_INFO("Got it/n");
	 	CvPoint p0 = cvPoint(cvRound(cornersA[i].x), cvRound(cornersA[i].y));
	 	CvPoint p1 = cvPoint(cvRound(cornersB[i].x), cvRound(cornersB[i].y));
		cvLine(imgC, p0, p1, CV_RGB(255,0,0),2);
	}
	cvNamedWindow("ImageA",0);
	cvNamedWindow("ImageB",0);
	cvNamedWindow("LKpyr_OpticalFlow",0);
	cvShowImage("ImageA",previous);
	cvShowImage("ImageB",next);
	cvShowImage("LKpyr_OpticalFlow",imgC);
}

/////////////////////////////////////////////     image callback     ////////////////////////////////////////////////////

void imageCallback(const sensor_msgs::ImageConstPtr& image){  //This function is called everytime a new image is published
	cv_bridge::CvImageConstPtr cv_ptr;
	try{
		cv_ptr = cv_bridge::toCvShare(image, "bgr8");
	} 
	catch(cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	if(aux != 0)  // Caso não seja a primeira imagem pegada da câmera, copia-se a imagem prévia
		image_rgb_previous = image_rgb.clone();
	image_rgb = cv_ptr->image;
	if(aux == 0)  // Caso seja a primeira imagem pegada da câmera, não há imagem prévia. Logo, se copia a imagem atual
		image_rgb_previous = image_rgb.clone();
	//opticalFlow(image_rgb_previous, image_rgb);
	
	namedWindow("Image Prévia"); 
	//imshow("Image Prévia", image_rgb_previous); 	


	creatTrackbarYellow();
	//creatTrackbarRed();
	filter();
        Contorno();
	findYellowBlocks(image_rgb, squaresYellow);
    	drawSquares(image_yellow, squaresYellow);
	draw();
	//int depth = ReadDepthData(RefPointYellowDepth.y, RefPointYellowDepth.x, image); // Width = 640, Height = 480		
	depth += ReadDepthData(260, 160, image); // Width = 640, Height = 480
	aux++;	
	if(aux%20 == 0){       
		ROS_INFO("Depth: %d", depth/20);
		aux = 0;
		depth = 0;
	}	
	//findRedBlocks(image_rgb, squaresRed);
	//drawSquares(image_red, squaresRed);
	namedWindow("Image Yellow"); 
	imshow("Image Yellow", image_yellow); 
	//namedWindow("Image Red"); 
	//imshow("Image Red", image_red); 
	// findContours(image_rgb);
	cv::waitKey(3);
}

/////////////////////////////////////////////// main ////////////////////////////////////////////////////////


int main(int argc, char **argv){

	ros::init(argc, argv, "image_processor");
        ros::NodeHandle nh;
	image_transport::ImageTransport it(nh); 
     	image_transport::Subscriber sub = it.subscribe("/camera/rgb/image_raw", 1, imageCallback); //********

	ros::Rate loop_rate(10);

	while (ros::ok()){
		  ros::spinOnce();
		  loop_rate.sleep();
	}

        pub = it.advertise("rgb/image_processed", 1);
        ros::spin();
   	ROS_INFO("tutorialROSOpenCV::main.cpp::No error.");
	return 0;
}

