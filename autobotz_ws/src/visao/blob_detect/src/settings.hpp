//A ideia aqui é implementar uma classe
//que centralize os parametros que devem ser mudados facilmente
//entre uma execução e outra do programa e durante a execução
//
//Os parâmetros são inicializados a partir de um arquivo ou de valores
//padrão escritos em código
//Uma interface grafica pode alterar os parametros no servidor de
//configuraçao e todas as funções de processamento de visao usarão os
//novos valores automaticamente
//
//Uma unica instancia da classe é criada globalmente

#ifndef _BLOB_DETECT_SETTINGS_HPP_
	#define _BLOB_DETECT_SETTINGS_HPP_

#include "vision.hpp"
//cpp
#include <vector>
#include <string>

using std::vector;

//make a single global object that will be our settings server
class settingsServer_T;
extern settingsServer_T settingsServer;
//example usage:
//	//assuming there is a variable 'myVar' declared in the body
//  //of the settingsServer_T class, one may write:
//	settingsServer.myVar = 2;


class settingsServer_T
{
	public:
		settingsServer_T();
		//constructor

		void updateSettingsFromFile(std::string fileName);
		//parses a settings file and write the new values
		//to the settings variables
		void updatePaletteFromFile(std::string fileName);
		//parses a palette file and write the new
		//values to targetPalette

		//TODO: maybe a way to get settings from ROS?

		//////////////// the settings (variables) ////////////////////
		// they can be acessed directly from anywhere in the code
		// UI functions should write updated values to them
		// Vision functions should read parameters from here instead
		// of using hard-coded values
		//////////////////////////////////////////////////////////////

		//used in getEdges function
			//canny edge detector parameters
			double getEdges_low_threshold ;
			double getEdges_high_threshold;
			int    getEdges_kernel_size   ;

		//used on main
			bool  MAIN_autoresize;   //specifies if the program sould resize the image to keep loop rate
			float MAIN_resize_factor;
			float MAIN_max_resize_factor;
			float MAIN_min_resize_factor;

		//paltte used in clipping colors to send to ROS
			palette targetPalette;

	private:
		//functions to help in parsing the configuration files

		int parsePaletteLine(std::string L, 
			                 int& B, int& G, int& R, float& maxDist, string& name);
		//extracts information from a string containing a line from a palette file
		//
		//returnss -1 if there was an error, 0 otherwise
};

#endif
