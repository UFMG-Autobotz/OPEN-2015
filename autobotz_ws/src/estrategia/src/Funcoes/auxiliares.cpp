#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auxiliares.hpp"

#define TELA_X 640.0
#define TELA_Y 480.0


/* ---------------------- FUNCAO LECONSTANTESARQUIVO ---------------------

    Entrada: Caminho para arquivo que contém as constantes e as variaveis
    para armazenar as 4 constantes de proporcional e derivativo 
    Saida: 1 ou -1. 1 indica fim do processo com sucesso
    Finalidade: Carregar as constantes de controle de um arquivo de 
    texto

-------------------------------------------------------------------------*/

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


/* ------------------ FUNCAO LOCALIZADESTINO -------------------

    Entrada: array com informacoes dos blocos dada pela visao
    Saida: posição x na tela onde o barco deve ir
    Finalidade: calcular o destino de navegacao do barco

----------------------------------------------------------*/

float localizaDestino(estrategia::featureVec blocos){


	float menor=TELA_X;
	float maior=0.0;
	float temp;

	int i;

 printf ("\n\n-------------- funcao---------------\n\n");
	for (i=0; i<blocos.features.size(); i++){


    	temp =  (float)blocos.features[i].centroid.x;

		if ((temp < menor) && (temp >= 0))
			menor = temp;
		

		if ((temp > maior) && (temp <= TELA_X))
			maior = temp;
	}
	printf ("\nRETORNO: %f - Maior: %f, Menor: %f\n", (maior + menor)/2.0, maior, menor);
	return ((float)(maior - menor)/2.0); // retorna centro da massa de blocos

}