///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//	The barco_launch package aims to provide tools to easily                 //
//  calibrate, start and debug the autobotz boat                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//
// O programa roda nos seguintes modos:
// 		nenhum argumento: pede para que um arduino seja plugado por vez, configurando
//                        assim que porta corresponde a cada um. Os nós do ROS são 
//		                  iniciados com as informações adquiridas.
//		rec: inicia o barco normalemnte mas grava todos os tópicos usando rosbag
//
//		calibra-us: roda procedimento de calibração dos ultrassons e grava os
//					 os resultados em um arquivo
//
//		calibra-imu: roda procedimento de calibração da imu

#include <

//funcoes correspondentes aos modos de operacao
void launch(bool rec);  //inicia barco, se rec == true, grava dados com rosbag
void calibra_us();    //calibra ultrassonico
void calibra_imu();   //calibra a imu


int main(int argc, char** argv)
{
	//////// tratar os argumentos passados ao programa
	char arg_rec    [] = "rec"; 	//strings de argumentos correspondentes aos modos
	char arg_cal_us [] = "calibra-us";
	char arg_cal_imu[] = "calibra-imu";

	bool has_rec     = false;  //flags de quais argumentos estão presentes
	bool has_cal_us  = false;
	bool has_cal_imu = false;

	//search for arguments
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(arg_rec, argv[i]))
			has_rec = true;
		else if(strcmp(argv[i], arg_cal_us))
			has_cal_us = true;
		else if(strcmp(argv[i], arg_cal_us))
			has_cal_imu = true;
	}
			
	//choose mode
	if(argc == 1)   //no params
		launch(rec);
	else if(has_cal_us)
		calibra_us();
	else if(has_cal_imu)
		calibra_imu();
	else
		printf("%s foi chamado com argumentos que não foram reconhecidos. Terminando...", argv[0]);


	return 0;
}

void launch(bool rec)
{
	
}

void calibra_us()
{
	
}

void calibra_imu()
{
	
}
