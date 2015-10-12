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


#include "transporte.h"
#include "estrategia/velocidade.h"

#endif




// -------------------- CONSTANTES --------------------

#define ARENA_X 500
#define ARENA_Y 300

#define RECUO 10 // em centimetros
#define RESOLUCAO 1 // cm

#define ANG_KP 0.7

#define ERRO_OK 5

#define PI 3.14159265

#define OBJ_X 370
#define OBJ_Y 150
#define OBJ_THETA 180

// ----------------- VARIÁVEIS GLOBAIS ------------------

float pos_x, pos_y;
float angulo_saida;


// ---------------- CALLBACK FUNCTIONs -----------------

void posicao (const geometry_msgs::Pose2D& msg){

    pos_x = msg.x;
    pos_y = msg.y;
    //    = msg.theta;

 }


/* ------------------ FUNCAO DESATRACAR -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica fim do processo
	Finalidade: Rotacionar o robo e deixá-lo pronto para seguir
	na direcao do proximo objetivo (porto/plataforma)

----------------------------------------------------------*/

int desatracar(Robo *barco){

	int i, delta_x, delta_y;
	int lado; 
	float vel_linear, vel_angular;

	/*

	// Faz um recuo
	for (i=0; i<RECUO/REOLUCAO; i++){

		barco.trajetoria.pontos[i] = barco.posicao.x - (barco.getLadoArena()) * i * RESOLUCAO;

	}

	*/

	// velocidade linear igual a zero, o barco so vai rodar
	vel_linear = 0.0;

	
	// definindo angulo de saida
	delta_y = barco->getObjetivo().y - barco->getPosicao().y;
	delta_x = barco->getObjetivo().x - barco->getPosicao().x;
	angulo_saida = atan(delta_y/delta_x) * 180 / PI;  // angulo em GRAUS
	// velocidade angular proporcional ao erro. Velocidade em GRAUS/SEGUNDO
	vel_angular = ANG_KP * (barco->getPosicao().theta - angulo_saida) * barco->getLadoArena(); // positivo: anti-horario; negativo: horario

	barco->setVelocidade(vel_linear, vel_angular);

	if (distancia2pts(barco->getPosicao(), barco->getObjetivo()) <= ERRO_OK)
		return 1;

	return 0;

}


/* ------------------ FUNCAO ATRACAR -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica fim do processo
	Finalidade: Rotacionar o robo e deixá-lo apto a atracar 
	no porto/plataforma

----------------------------------------------------------*/

int atracar(Robo *barco){


	return 0;
}



/* ------------------ FUNCAO ATRACAR -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: Nada.
	Finalidade: Setar a posição na qual o barco estará se
	movendo nos próximos instantes

----------------------------------------------------------*/

void defineObjetivo(Robo *barco){

	// Sempre mesmo ponto durante a prova
	barco->setObjetivo(OBJ_X, OBJ_Y, OBJ_THETA);

}



// ----------------------- AUXILIARES -----------------------


/* ------------------ FUNCAO DISTANCIA2PTS -------------------

	Entrada: 2 pontos do tipo geometry_msgs::Pose2D
	Saida: distância (float) entre os dois pontos de entrada
	Finalidade: Calcular distância cartesiana entre dois
	pontos quaisquer

----------------------------------------------------------*/

float distancia2pts (geometry_msgs::Pose2D pt1, geometry_msgs::Pose2D pt2){


	float distancia;

	distancia = sqrt( pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2) );

	return distancia;

}