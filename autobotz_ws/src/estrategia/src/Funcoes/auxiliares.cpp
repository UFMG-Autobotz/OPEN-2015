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
