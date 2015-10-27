#include <stdio.h>
#include <string>
#include "estrategia/featureVec.h"

int leConstantesArquivo(std::string diretorio, float *linear_kp, float *linear_kd, float *angular_kp, float *angular_kd);

float localizaDestino(estrategia::featureVec blocos);
