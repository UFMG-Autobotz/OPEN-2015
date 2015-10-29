#include <stdio.h>
#include <string>
#include <vector>


#include "ros/ros.h"
#include "estrategia/featureVec.h"
#include "estrategia/feature.h"
#include "std_msgs/String.h"


int leConstantesArquivo(std::string diretorio, float *linear_kp, float *linear_kd, float *angular_kp, float *angular_kd);

float localizaDestino(estrategia::featureVec blocos, std::vector<float> *blocos_anteriores);

estrategia::feature escolherBloco(estrategia::featureVec blocos);

/* ------------------ FUNCAO TrackBloco -------------------

    Entrada: array com informacoes dos blocos dada pela visao
    Saida: posição x na tela onde o braco deve ir
    Finalidade: acompanhar o bloco a ser pego pela garra apesar de variações na 
                posição e "piscadas" entre um frame e outro
----------------------------------------------------------*/
float trackBloco(estrategia::featureVec blocos);