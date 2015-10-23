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


#include "transportarBloco.h"
#include "estrategia/velocidade.h"

#endif




// -------------------- CONSTANTES --------------------

#define ARENA_X 500
#define ARENA_Y 300

#define RECUO 10 // em centimetros
#define RESOLUCAO 1 // cm

#define VEL_ANG_KP 1.0
#define VEL_LIN_KP 1.0 // isso não tem relação com controle, é só pra determinar a curva da velocidade

#define ERRO_LIN_OK 5
#define ERRO_ANG_OK 5

#define PI 3.14159265

#define PORT_OBJ_X 120
#define PORT_OBJ_Y 75
#define PORT_OBJ_THETA 180

#define PLAT_OBJ_X 370
#define PLAT_OBJ_Y 150
#define PLAT_OBJ_THETA 0

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

int desatracar(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo){

	int i, delta_x, delta_y;
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
	delta_y = posicao_objetivo->y - barco->getPosicao().y;
	delta_x = posicao_objetivo->x - barco->getPosicao().x;
	angulo_saida = atan(delta_y/delta_x) * 180 / PI;  // angulo em GRAUS
	// velocidade angular proporcional ao erro. Velocidade em GRAUS/SEGUNDO
	vel_angular = VEL_ANG_KP * (barco->getPosicao().theta - angulo_saida) * barco->getLadoArena(); // positivo: anti-horario; negativo: horario

	barco->setVelocidadeBarco(vel_linear, vel_angular);

	if ((barco->getPosicao().theta - posicao_objetivo->theta) <= ERRO_ANG_OK)
		return 1;

	return 0;

}


/* ------------------ FUNCAO FAZTRAJETORIA -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica fim do processo
	Finalidade: Determina caminho a ser seguido e faz o trajeto

----------------------------------------------------------*/

int fazTrajetoria(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo){


	int i, delta_x, delta_y;
	float delta_d;
	float theta, traj_x, traj_y;
	float vel_linear, vel_angular;

	// definindo angulo trajetoria
	delta_y = posicao_objetivo->y - barco->getPosicao().y;
	delta_x = posicao_objetivo->x - barco->getPosicao().x;
	theta = atan(delta_y/delta_x) * 180 / PI;  // angulo em GRAUS;

	barco->limpaTrajetoria();

	// calculando distância
	delta_d = distancia2pts(barco->getPosicao(), *posicao_objetivo);

	for (i=1; i <= (delta_d / RESOLUCAO); i++){

		traj_x = cos(RESOLUCAO)*i+barco->getPosicao().x;
		traj_y = sin(RESOLUCAO)*i+barco->getPosicao().y;


		barco->addPontoTrajetoria(traj_x, traj_y, theta);

	}

	// velocidade linear igual a zero, o barco so vai rodar
	vel_angular = 0.0;

	


	
	// velocidade linear proporcional ao erro. Velocidade em CENTIMETROS/SEGUNDO
	vel_linear = VEL_LIN_KP * (1/delta_d) * barco->getLadoArena(); // positivo: anti-horario; negativo: horario

	
	barco->setVelocidadeBarco(vel_linear, vel_angular);

	if (distancia2pts(barco->getPosicao(), *posicao_objetivo) <= ERRO_LIN_OK)
		return 1;

	return 0;


}

/* ------------------ FUNCAO ATRACAR -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: 1 ou 0. 1 indica fim do processo
	Finalidade: Rotacionar o robo e deixá-lo apto a atracar 
	no porto/plataforma

----------------------------------------------------------*/

int atracar(Robo *barco, geometry_msgs::Pose2D *posicao_objetivo){


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
	delta_y = posicao_objetivo->y - barco->getPosicao().y;
	delta_x = posicao_objetivo->x - barco->getPosicao().x;
	angulo_saida = atan(delta_y/delta_x) * 180 / PI;  // angulo em GRAUS
	// velocidade angular proporcional ao erro. Velocidade em GRAUS/SEGUNDO
	vel_angular = VEL_ANG_KP * (barco->getPosicao().theta - angulo_saida) * barco->getLadoArena(); // positivo: anti-horario; negativo: horario

	barco->setVelocidadeBarco(vel_linear, vel_angular);

	if ((barco->getPosicao().theta - posicao_objetivo->theta) <= ERRO_ANG_OK)
		return 1;


	return 0;
}



/* ------------------ FUNCAO ATRACAR -------------------

	Entrada: Objeto do tipo Robo (classe definida em robo.cpp)
	Saida: Nada.
	Finalidade: Setar a posição na qual o barco estará se
	movendo nos próximos instantes

----------------------------------------------------------*/



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