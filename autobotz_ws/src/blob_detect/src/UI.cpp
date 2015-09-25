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

#include "UI.hpp"


using namespace cv;
using namespace std;

////////////////////  variaveis globais  /////////////////////////

// Declaração e inicialização dos valores HSV do Trackbar para os Blocos Amarelos
extern int H_yellow_min, H_yellow_max, 
           S_yellow_min, S_yellow_max,
           V_yellow_min, V_yellow_max; 

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
