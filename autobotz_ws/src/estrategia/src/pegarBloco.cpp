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


#include "pegarBloco.h"// estado AGARRAR


#endif




// -------------------- CONSTANTES --------------------

#define DELAY_GARRA 2 // em segundos
#define VEL_GARRA 70.0
#define VEL_LIN_BRACO 100
#define VEL_LIN_BRACO_DEVAGAR 50 
#define VEL_ANG_BRACO_KP 1.5
#define ERRO_ANG_OK 5

#define TEMPO_PEGA_BLOCO 1000 // ms

// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------



/* ------------------ FUNCAO ESTENDERBRACO -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp), variável que indica
	se primeiro sensor ótico leu bloco dentro da garra, variável que indica se 
	segundo leitor óptico leu bloco dentro da garra
	Saida: 1 ou 0. 1 indica que a o bloco já está todo dentro da garra
	Finalidade: Determina caminho a ser seguido e faz o trajeto

------------------------------------------------------------*/


int estenderBraco(Robo *barco, bool tem_bloco, float referencia_ang){


	float vel_ang = 0.0;

	// se o sensor ainda não está lendo bloco dentro da garra, estende o braço com velocidade constante e continua nesse estado
	if (!tem_bloco){

		vel_ang = referencia_ang * VEL_ANG_BRACO_KP;

		// seta velocidade linear e deixa velocidade angular em zero
		barco->setVelocidadeBraco(VEL_LIN_BRACO, vel_ang);
		return 0;
	}

	else if (tem_bloco)
	{
		sleep(TEMPO_PEGA_BLOCO);
		// para de mexer o braco, ja que alcançou o bloco
		barco->setVelocidadeBraco(0, 0);
		return 1;
	}

	return 0;
}



/* ------------------ FUNCAO RECOLHERBRACO -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica que a garra já está de volta dentro do barco
	Finalidade: Recolher a garra depois de ter pego um bloco

------------------------------------------------------------*/

int recolherBraco(Robo *barco){


		// seta velocidade linear e deixa velocidade angular em zero
		barco->setVelocidadeBraco((-1)*VEL_LIN_BRACO, 0);		
		return 1;

}



/* ------------------ FUNCAO AGARRABLOCO -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica que a o bloco já está preso de maneira segura na garra
	Finalidade: Fechar a garra e prender o bloco

------------------------------------------------------------*/

int agarrarBloco(Robo *barco){

	// velocidade angular da garra igual a 0
	barco->setVelocidadeGarra(VEL_GARRA, 0);

	return 1;

}


/* ------------------ FUNCAO GUARDARBLOCO -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp); angulo que a 
	base do braço sera girada para guardar a garra
	Saida: 1 ou 0. 1 indica que a garra já esta guardada
	Finalidade: colocar a garra em posicao segura dentro do barco, para então
	proteger o bloco que acabou de ser pego

------------------------------------------------------------*/

int guardarBraco(Robo *barco, float ang_recolhimento){


	// foi setado velocidade angular constante, mas é possivel fazer um proporcional, talvez seja melhor
	//barco->setVelocidadeGarra(0, VEL_ANG_BRACO);

	// para e retorna sucesso quando o erro eh aceitavel
	if (barco->getPosicao().theta - ang_recolhimento <= ERRO_ANG_OK){
		barco->setVelocidadeGarra(0, 0);
		return 1;
	}


	return 0;

}