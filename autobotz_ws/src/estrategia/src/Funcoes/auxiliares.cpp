#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


#include "auxiliares.hpp"

#define TELA_X 640.0
#define TELA_Y 480.0

#define LIM_BLOCO_CIMA 480
#define LIM_BLOCO_BAIXO 0

#define MEDIDAS_LEMBRADAS 15


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

float localizaDestino(estrategia::featureVec blocos, std::vector<float> *blocos_anteriores){


	float media_atual=0.0, media_final=0.0;
	float tempY, tempX;
	int i, cont=0;




	// le todos os blocos que esta vendo no momento
	for (i=0; i<blocos.features.size(); i++){


    	tempX =  (float)blocos.features[i].centroid.x;
    	tempY =  (float)blocos.features[i].centroid.y;

    	// ignora faixa para cima e para baixo onde nao deveria ter bloco
    	if ((tempY < LIM_BLOCO_CIMA) && (tempY > LIM_BLOCO_BAIXO) && blocos.features[i].colorName.data.compare("yellow")){

    		// media das posições X de todos os blobs visiveis
    		media_atual += tempX;
    		cont++;
    	
		}

	}
	
	if (cont != 0)
		media_atual = media_atual / (float)cont;

	printf ("\n\n################  MEDIA ATUAL: %f  CONT: %d ################\n\n", media_atual, cont);

	if (cont != 0){
		// adiciona no vetor de medidas lembradas. Isso server para eliminar as medidas falsas 
		if (blocos_anteriores->size() >= MEDIDAS_LEMBRADAS){

			// mantem o vetor de medidas anteriores no limite, apagando medidas mais antigas e adicionando recentes
			blocos_anteriores->erase(blocos_anteriores->begin());
			blocos_anteriores->push_back(media_atual);
			

		}
		else {
			blocos_anteriores->push_back(media_atual);
		
		}
	}




	for (i=0; i<blocos_anteriores->size(); i++){

		media_final += blocos_anteriores->at(i);

	}

	if (blocos_anteriores->size() != 0)
		media_final /= blocos_anteriores->size();

printf("\nTAM=%li\n", blocos_anteriores->size());
	for (i=0; i<blocos_anteriores->size(); i++){

			
			printf ("%.2f, ", blocos_anteriores->at(i));
			
	}

	printf ("\n\n################  MEDIA FINAL: %f ################\n\n", media_final);

	return media_final; // retorna media das ultimas medidas de destino

}

/* ------------------ FUNCAO TrackBloco -------------------

    Entrada: array com informacoes dos blocos dada pela visao
    Saida: posição x na tela onde o braco deve ir
    Finalidade: acompanhar o bloco a ser pego pela garra apesar de variações na 
                posição e "piscadas entre um frame e outro"

----------------------------------------------------------*/
float trackBloco(estrategia::featureVec blocos, bool &procurando)
{
	static float POI_x = TELA_X/2; //current Point Of Interest for the boat
	static float POI_y = TELA_Y/2;

	static float radius = 10;  //radius of search for new blobs

	float r_min = 10;     // radius resets to r_min if a blob is found inside search region
	float r_max = TELA_X; // 
	float inc   = 3;      // each time the function is called and there is no blob inside radius, radius += inc;

	procurando = false;

	//search for blobs inside the radius around the cur POI
	for(int i = 0; i < blocos.features.size(); i++)
	{
		//get distance
		float x = blocos.features[i].centroid.x;
		float y = blocos.features[i].centroid.y;

		float dist = sqrt(x*x + y*y);

		if(dist < radius)  //we still have a point inside our previous region of interest
		{
			POI_x = x;
			POI_y = y;

			radius = r_min;

			procurando = true;

		}
		else   //no blob, let's search wider
		{
			//keep last POI x and y

			radius += inc;//and increment search radius
			if(radius > r_max) radius = r_max;
		}
	}

	return POI_x;
}


