#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auxiliares.hpp"

int leConstantesArquivo(std::string diretorio, float *linear_kp, float *linear_kd, float *angular_kp, float *angular_kd){


	char linha[20];
	FILE *arq;

	arq = fopen(diretorio.c_str(), "r");

	if (arq == NULL){
		printf ("\nErro ao tentar abrir o arquivo %s!\n", diretorio.c_str());
		return -1;
	}

	

	while (!feof(arq) && strcmp(linha, "LINEAR")!=0){

		fscanf(arq, "%s", linha);
	}

	fscanf(arq, "%s", linha); 
	fscanf(arq, "%f", linear_kp); 
	fscanf(arq, "%s", linha); 
	fscanf(arq, "%f", linear_kd);  

	rewind(arq);

	while (!feof(arq) && strcmp(linha, "ANGULAR")!=0){

		fscanf(arq, "%s", linha);
		
	}
	
	fscanf(arq, "%s", linha);
	fscanf(arq, "%f", angular_kp);
	fscanf(arq, "%s", linha);
	fscanf(arq, "%f", angular_kd);

	return 1;



}


/* ------------------ FUNCAO DISTANCIAANGULARIMU -------------------

    Entrada: 2 angulos dados em graus
    Saida: menor distancia angular entre eles
    Finalidade: consertar a descontinuidade da IMU
----------------------------------------------------------*/

float distanciaAngularIMU (float angulo1, float angulo2){

	float dist1_0, dist1_180, dist2_0, dist2_180;
	float dist0, dist180;

	dist1_0 = angulo1 - 0;

	dist1_180 = angulo1 - 180;

	dist2_0 = angulo2 - 0;

	dist2_180 = angulo2 - 180;

	dist0 = abs(dist2_0) + abs(dist1_0);
	dist180 = abs(dist1_180) + abs(dist2_180);



	if (dist0 < dist180){

		// angulo2: referencia, angulo1: YAW
		if (angulo2 < angulo1)
			dist0 *= -1;
	
		return dist0;
	
	}

	// angulo2: referencia, angulo1: YAW
	if (angulo2 < angulo1)
		dist180 *= -1;
	
	return dist180;


}
