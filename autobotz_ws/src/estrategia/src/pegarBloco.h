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

int estenderBraco(Robo *barco, int tem_bloco[2]);

int agarrarBloco(Robo *barco);

int recolherBraco(Robo *barco);

int guardarBraco(Robo *barco, float ang_recolhimento);