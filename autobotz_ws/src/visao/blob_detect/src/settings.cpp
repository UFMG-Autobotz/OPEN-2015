//implement the settings server

#include "settings.hpp"
//cpp
#include <string>

using std::string;

//actually create the global settings server object
settingsServer_T settingsServer;

//class constructor
settingsServer_T::settingsServer_T()
{
	/////////////  set some hard-coded default values  /////////////

	//used on the getEdges function 
	getEdges_low_threshold  = 100;
	getEdges_high_threshold = 3*getEdges_low_threshold;
	getEdges_kernel_size    = 5;

	///////////////// read custom values from some file ////////////
	//TODO, use updateFromFile here	
}

//update class variables from file
void settingsServer_T::updateFromFile(string fileName)
{
	//TODO: implementation
}
