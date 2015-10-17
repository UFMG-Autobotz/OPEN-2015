/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
(31) 9405-5012
gustdomar@gmail.com

Código principal do pacote de estratéiga
*/



// -------------------- BIBLIOTECAS --------------------

#include <math.h>
#include <geometry_msgs/Pose2D.h>

// ---------------- ARQUIVOS INCLUSOS ------------------


#ifndef INCLUIR
#define INCLUIR


#include "pegarBloco.h"


#endif




// -------------------- CONSTANTES --------------------

#define VEL_LIN_BRACO 150
#define VEL_LIN_BRACO_DEVAGAR 50
#define VEL_ANG_BRACO 60

// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------



/* ------------------ FUNCAO ESTENDERBRACO -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp), variável que indica
	se primeiro sensor ótico leu bloco dentro da garra, variável que indica se 
	segundo leitor óptico leu bloco dentro da garra
	Saida: 1 ou 0. 1 indica que a o bloco já está todo dentro da garra
	Finalidade: Determina caminho a ser seguido e faz o trajeto

------------------------------------------------------------*/


int estenderBraco(Robo *barco, int tem_sensor[2]){


	// se ainda nenhum dos sensores está lendo bloco dentro da garra, estende o braço com velocidade constante e continua nesse estado
	if (!tem_sensor[0] && !tem_sensor[1]){

		// seta velocidade linear e deixa velocidade angular em zero
		barco->setVelocidadeBraco(VEL_LIN_BRACO, 0);
		return 0;
	}

	else if (tem_sensor[0] && !tem_sensor[1]){

		// seta velocidade linear e deixa velocidade angular em zero
		barco->setVelocidadeBraco(VEL_LIN_BRACO_DEVAGAR, 0);
		return 0;
	}

	else if (tem_sensor)
	{
		// para de mexer o braco, ja que alcançou o bloco
		barco->setVelocidadeBraco(0, 0);
		return 1;
	}

	return 0;
}

