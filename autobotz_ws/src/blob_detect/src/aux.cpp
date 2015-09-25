#include <ros/ros.h>	
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>	
#include <sensor_msgs/image_encodings.h>	
#include <opencv2/imgproc/imgproc.hpp>	
#include <opencv2/highgui/highgui.hpp>	
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  
#include <string.h> 
#include <iostream> 
#include <sensor_msgs/Image.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <X11/keysym.h>
#include <cv.h> 
#include <highgui.h>
#include <cxcore.h>
#include "opencv/cvaux.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include "opencv2/core/core.hpp"
#include <sstream>

#include "aux.hpp"


using namespace cv;
using namespace std;

////////////////////  variaveis globais  /////////////////////////

// Declaração e inicialização dos valores HSV do Trackbar para os Blocos Amarelos
extern int H_yellow_min, H_yellow_max, 
           S_yellow_min, S_yellow_max,
           V_yellow_min, V_yellow_max; 

extern Mat binary, image_rgb, image_rgb_previous, image_merge, image_yellow, image_red; 


Mat Rchannel, Gchannel, Bchannel; // Variáveis que guardarão cada um dos 3 canais da imagem HSV

Point RefPointYellow;
Point RefPointYellowDepth;

/////////////////////////////// filter() ////////////////////////////////
void filter()
{
	std::vector<cv::Mat> three_channels;// Vetor para comportar cada os 3 canais

	////// Divide a imagem RGB em 3 canais
	split(image_rgb, three_channels);   
 	
 	Rchannel = three_channels[0];
	Gchannel = three_channels[1];
	Bchannel = three_channels[2];

	////// Apply morphological transforms (erosion and dilation)

	//Create kernels
	Mat erodeElement = getStructuringElement(MORPH_RECT,Size(3,3));  // Definição da estrutura do elemento a ser erodido	
	Mat dilateElement = getStructuringElement(MORPH_RECT,Size(8,8));  // Definição da estrutura do elemento a ser dilatado
	
	//R channel
	erode (Rchannel, Rchannel, erodeElement );
	dilate(Rchannel, Rchannel, dilateElement);
	//G channel
	erode (Gchannel, Gchannel, erodeElement );
	dilate(Gchannel, Gchannel, dilateElement);
	//B channel
	erode (Bchannel, Bchannel, erodeElement );
	dilate(Bchannel, Bchannel, dilateElement);


	////// merge the channels back
	merge(three_channels, image_merge);
}

/////////////////////////////// contorno() ///////////////////////////////
void contorno()
{
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
    	cv::RNG rng(12345);
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

/////////////////////////////// findContours() ///////////////////////////////
void findContours(Mat image_rgb)
{
	// Converte a imagem original para "grayscale"
	cv::Mat gray;
	cv::cvtColor(image_rgb, gray, CV_BGR2GRAY);

	// Converte para uma imagem binária utilizando Canny
	cv::Mat bw;
	cv::Canny(gray, bw, 1000, 3000, 5);  // OBS: Esses parâmetros podem ser modificados

        // Exibe na tela a imagem em escala binária
   	cv::namedWindow("Grayscale Image"); 
	cv::imshow("Grayscale Image", bw);

	// Encontra contornos
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// O array para guardar a curva aproximada
	std::vector<cv::Point> approx;

	// Declaração de uma imagem para colocarmos o texto resultante
	cv::Mat dst = image_rgb.clone();

	int i = 0;  // Contador para ser utilizado no loop

	for (i = 0; i < contours.size(); i++){

		// Approximate contour with accuracy proportional
    		// to the contour perimeter
    		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

    		// Skip small or non-convex objects 
    		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        		continue;

		if (approx.size() == 3)
 		       	setLabel(dst, "TRI", contours[i]);    // Triangles
	
		else if (approx.size() >= 4 && approx.size() <= 6){
        
			// Number of vertices of polygonal curve
		        int vtc = approx.size();

 		       	// Get the degree (in cosines) of all corners
 		       	std::vector<double> cos;
 		       	for (int j = 2; j < vtc+1; j++)
 		       		cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

 	    		// Sort ascending the corner degree values
 	      		std::sort(cos.begin(), cos.end());

 		       	// Get the lowest and the highest angle
 		       	double mincos = cos.front();
 		       	double maxcos = cos.back();

 		       	// Use the degrees obtained above and the number of vertices
 		       	// to determine the shape of the contour
 		       	if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3){
 	           		// Detect rectangle or square
        	    		cv::Rect r = cv::boundingRect(contours[i]);
        	    		double ratio = std::abs(1 - (double)r.width / r.height);

        	    		setLabel(dst, ratio <= 0.02 ? "SQU" : "RECT", contours[i]);
        		}
        		else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
        	    		setLabel(dst, "PENTA", contours[i]);
        		else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
        	    		setLabel(dst, "HEXA", contours[i]);
    		}

		else{
        		// Detect and label circles
        		double area = cv::contourArea(contours[i]);
        		cv::Rect r = cv::boundingRect(contours[i]);
        		int radius = r.width / 2;
	
        		if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 && std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2){
        			setLabel(dst, "CIR", contours[i]);
        		}
    		}
	}

	// Exibe a imagem original
	cv::namedWindow("Image"); 
	cv::imshow("Image", image_rgb);

	// Exibe a imagem processada
	cv::namedWindow("Final Image"); 
	cv::imshow("Final Image", dst);
}

/////////////////////////////   findYellowBlocks   ////////////////////////////////

void findYellowBlocks(const Mat& image, vector< vector<Point> >& squares)
{
	squares.clear();  // Limpa o vetor de pontos

	Mat gray, image_hsv;

	image_rgb.copyTo(image_yellow);

	vector<vector<Point> > contours;
	vector<Point> approx;

	cv::cvtColor(image, image_hsv, CV_BGR2HSV); 

	cv::inRange(image_hsv, Scalar(H_yellow_min, S_yellow_min, V_yellow_min), Scalar(H_yellow_max, S_yellow_max, V_yellow_max), gray);	
	cv::namedWindow("Yellow Adjustment");
    cv::imshow("Yellow Adjustment", gray);

	// Encontra os contornos e os guarda em uma lista
	cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	/*
	for(int l = 0; l < contours.size(); l++)
		for(int m = 0; m < contours.size(); m++)
			ROS_INFO("ID = %i %i\n", contours[l][m].x, contours[l][m].y);*/
	
	// Testa cada contorno
	for(size_t i = 0; i < contours.size(); i++)
	{
		// Aproxima o contorno com acurácia proporcional ao seu perímetro
		cv::approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		// Os contornos retângulos devem ter 4 vértices após a aproximação e serem convexos. OBS: Os valores absolutos 			devem ser usados porque a área pode ser positiva ou negativa, dependendo da orientação do contorno
		if(approx.size() == 4 && fabs(contourArea(Mat(approx))) > 100 && isContourConvex(Mat(approx)))
		{
    		double maxCosine = 0;
           	for(int j = 2; j < 5; j++)
           	{
				// Encontra o máximo cosseno do ângulo entre duas juntas
           		double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
           		maxCosine = MAX(maxCosine, cosine);
        	}
			// Se os cossenos de todos os ângulos são pequenos (todos os ângulos são ~90 graus) então escreve os 				vértices na sequência resultante
			if(maxCosine < 0.26)
			{
           		squares.push_back(approx);
				/*for(int c = 0; c < approx.size(); c++)
	  			ROS_INFO("ID = %i %i\n", approx[c].x, approx[c].y);*/
				
				RefPointYellow.x = 0;
				RefPointYellow.y = 0;
				for(int g = 1; g <= approx.size(); g++)
				{
					RefPointYellow.x += approx[g].x; 
					RefPointYellow.y += approx[g].y;  
					if(g%4 == 0)
					{
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

///////////////////////////// auxiliar functions /////////////////////////
//////////////////////////////////////////////////////////////////////////

double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/////////////////////

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{  // Parâmetros: Imagem, Texto, Contorno
	int fontface = cv::FONT_HERSHEY_SIMPLEX;  // Definição na fonte utilizada
	double scale = 0.4; // Tamanho da letra
	int thickness = 1;  // Espessura da letra
	int baseline = 0;  // Parâmetetro de saída - coordenada y do "baseline" relativo ao ponto do texto mais baixo

	// void cvGetTextSize(const char* text_string, const CvFont* font, CvSize* text_size, int* baseline)
	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline); // Obtém o tamanho do texto
	cv::Rect r = cv::boundingRect(contour);  // Declaração de um retângulo

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	
	// void cvPutText(CvArr* img, const char* text, CvPoint org, const CvFont* font, CvScalar color)
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

////////////////////

void drawSquares(Mat& image, const vector<vector<Point> >& squares)
{
    	for(size_t i = 0; i < squares.size(); i++){
        	const Point* p = &squares[i][0];
        	int n = (int)squares[i].size();
        	polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
    	}
}

//////////////////////

void createTrackbarYellow()
{
	namedWindow("HSV Yellow Adjustment", CV_WINDOW_AUTOSIZE); 
	cvCreateTrackbar("H_yellow_min", "HSV Yellow Adjustment", &H_yellow_min, 255); 
  	cvCreateTrackbar("H_yellow_max", "HSV Yellow Adjustment", &H_yellow_max, 255); 
  	cvCreateTrackbar("S_yellow_min", "HSV Yellow Adjustment", &S_yellow_min, 255); 
  	cvCreateTrackbar("S_yellow_max", "HSV Yellow Adjustment", &S_yellow_max, 255); 
  	cvCreateTrackbar("V_yellow_min", "HSV Yellow Adjustment", &V_yellow_min, 255); 
  	cvCreateTrackbar("V_yellow_max", "HSV Yellow Adjustment", &V_yellow_max, 255); 
}
