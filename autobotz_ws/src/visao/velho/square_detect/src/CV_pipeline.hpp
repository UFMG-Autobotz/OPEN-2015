#ifndef _CV_PIPELINE_HPP_
	#define _CV_PIPELINE_HPP_

#include <vector>
#include "opencv2/core/core.hpp"

using namespace std;
using cv::Mat;
using cv::Point;

////////////////   main vision functions    //////////////////

void filter(Mat in_rgb, Mat out);
// applies erosion and dilation filter to each channel of the HSV image
//
// takes a rgb image as input and returns the output on the
// 'out' matrix


void contorno(Mat in_rgb);
// does: ???
//
// takes a rgb image as input

void findYellowBlocks(const Mat& image, vector< vector<Point> >& squares);
//find yellow blocks in the image

void findShapes(Mat image_rgb);
//Search for polygons (triengles, quadrangles and other) in the input image
//also outputs stuff to the screen

///////////////  some auxiliary functions  ///////////////////////

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	
void findContours(Mat image_rgb);

/////////////////    Data structures   /////////////////////

typedef union U_FloatParse{
	    float float_data;
	    unsigned char byte_data[4];
}U_FloatConvert;

#endif

///////////////////////////////////////////////////////////////////////
// Algumas funcoes e algoritmos que estao sendo usados nesse codigo  //
///////////////////////////////////////////////////////////////////////

//canny
//in : imagem greyscale
//out: imagem PB com bordas

//findcontours
//in : imagem PB com bordas
//out: vetor de cnotornos, onde cada contorno é um conjunto de pontos

//approxPolyDP
//in : contorno
//out: contorno com menos pontos
//
//notas: -usa o algoritmo de douglas-peucker para retirar pontos da linha
//       -arcLength, contourArea e isContourConvex tambem sao usadas
//        para selecionar os contornos

//filter
//in : imagem RGB
//out: imagem HSV alterada
//
//combinacao de uma operacao erode e uma dilate sobre cada
//canal da imagem

/////// pipes

//mostrar a imagem de contornos
//
//RGB -> GS -> canny -> findContours -> drawContours

//findYellowBlocks   (funcao sendo usada para publicar no ROS)
//
//RGB -> GS -> thresholding -> findConoturs -> approxPolyDP -> criterios (tamanho, convexo, angulos) -> lista de quadrados

//findShapes
//
//igual findYellowBlocks mas com criterios diferentes
