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

// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"



// -------------------- CABEÇALHOS --------------------


//void pegarBloco(int estado_atual);

//void colocarBloco(int estado_atual);

void transportarBloco(int *estado_atual, Robo barco);