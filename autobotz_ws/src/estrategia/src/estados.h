/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

Descrição do arquivo
*/



// -------------------- BIBLIOTECAS --------------------

#include <stdio.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>

// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"



// -------------------- CABEÇALHOS --------------------


int pegarBloco(int *estado_atual, Robo *barco, bool tem_bloco, bool agarrado);

//int colocarBloco(int *estado_atual, Robo *barco);

int transportarBloco(int *estado_atual, Robo *barco, geometry_msgs::Pose2D *posicao_objetivo);


