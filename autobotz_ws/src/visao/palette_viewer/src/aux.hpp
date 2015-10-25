//Arquivo com funções e estruturas de dados usadas para
//visualizar as paletas
//
#ifndef _AUX_HPP_
	#define _AUX_HPP_

#include "palette.hpp"

#include <opencv2/core/core.hpp>

#include <vector>
#include <string>

using std::vector;
using std::string;

////////////////////////// reading the palette file  //////////////////////////////

int updatePaletteFromFile(string fileName, palette& P);
//parses a palette file and write the new
//values to targetPalette
//
//returns 0 if ok, -1 if could not open file

int parsePaletteLine(std::string L, 
	                 int& B, int& G, int& R, float& maxDist, string& name);
//extracts information from a string containing a line from a palette file
//
//returnss -1 if there was an error, 0 otherwise

/////////////////   drawing the palette to an openCV image   ///////////////

void drawPalette(cv::Mat& canvas, const palette& P, int b);
//draws a palette into an openCV matrix
//
//the matrix shows a slice of the RGB color space (which is a cube)
//at heigth b

#endif
