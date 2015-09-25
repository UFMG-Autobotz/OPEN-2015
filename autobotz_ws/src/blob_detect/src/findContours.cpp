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

using namespace cv;
using namespace std;

// Método que auxilia na disposição do texto no centro do contorno
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour){  // Parâmetros: Imagem, Texto, Contorno
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

// Método que retorna o cosseno do ângulo entre dois vetores: pt0->pt1 e pt0->pt2, sendo pt (ponto)
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0){  // Recebe 3 Pontos como parâmetro
	double dx1 = pt1.x - pt0.x;  // x1 - x0
	double dy1 = pt1.y - pt0.y;  // y1 - y0
	double dx2 = pt2.x - pt0.x;  // x2 - x0
	double dy2 = pt2.y - pt0.y;  // y2 - y0
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void findContours(Mat image_rgb){

	// Converte a imagem original para "grayscale"
	cv::Mat gray;
	cv::cvtColor(image_rgb, gray, CV_BGR2GRAY);

	// Converte para uma imagem binária utilizando Canny
	cv::Mat bw;
	cv::Canny(gray, bw, 1000, 3000, 5);  // OBS: Esses parâmetros podem ser modificados

        // Exibe na tela a imagem em escala binária
   	namedWindow("Grayscale Image"); 
	imshow("Grayscale Image", bw);

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

 		       	// Get the lowest and the highest degree
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
	namedWindow("Image"); 
	imshow("Image", image_rgb);

	// Exibe a imagem processada
	namedWindow("Final Image"); 
	imshow("Final Image", dst);
}
