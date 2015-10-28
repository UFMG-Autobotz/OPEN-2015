#include <stdio.h>
#include <string>
#include <vector>

#include "estrategia/featureVec.h"

int leConstantesArquivo(std::string diretorio, float *linear_kp, float *linear_kd, float *angular_kp, float *angular_kd);

float localizaDestino(estrategia::featureVec blocos, std::vector<float> *blocos_anteriores);

estrategia::feature escolherBloco(estrategia::featureVec blocos);

estrategia::feature trackBloco(estrategia::featureVec blocos, estrategia::feature blocoObjetivo);