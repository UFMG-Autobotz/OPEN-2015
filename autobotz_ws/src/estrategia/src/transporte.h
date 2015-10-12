/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

*/



// -------------------- BIBLIOTECAS --------------------

#include <ros/ros.h>

// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"



// -------------------- CABEÇALHOS --------------------

int desatracar(Robo *barco);

void defineObjetivo(Robo *barco);

int atracar(Robo *barco);

// auxiliares

float distancia2pts (geometry_msgs::Pose2D pt1, geometry_msgs::Pose2D pt2);