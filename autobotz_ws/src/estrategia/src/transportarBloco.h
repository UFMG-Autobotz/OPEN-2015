/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

*/



// -------------------- BIBLIOTECAS --------------------

#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>

// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"



// -------------------- CABEÇALHOS --------------------

int desatracar(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo);

void defineObjetivo(Robo *barco);

int fazTrajetoria(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo);

int atracar(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo);


// auxiliares

float distancia2pts (geometry_msgs::Pose2D pt1, geometry_msgs::Pose2D pt2);