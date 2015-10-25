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
#include <geometry_msgs/Pose2D.h>

// ---------------- ARQUIVOS INCLUSOS ------------------

#ifndef INCLUIR
#define INCLUIR

#include "robo.h"
#include "estados.h"
#include "transportarBloco.h"
#include "pegarBloco.h"


#include "estrategia/velocidade.h"
#include "./Funcoes/auxiliares.hpp"


#endif

// -------------------- CONSTANTES --------------------

#define ANG_RECOLHIMENTO -120


// -------------------- CONSTANTES CONTROLE --------------------

#define TELA_LIN 480
#define TELA_COL 640
#define VEL_LIN_MAX 180
#define VEL_LIN_MIN -180
#define VEL_ANG_MAX 150
#define VEL_ANG_MIN -150
#define VEL_MAX 255
#define VEL_MIN -255
#define VEL_NOR 80 // intervalo de -255 a 255
#define VEL_RET 100
//#define angular_kp 1.2
//#define angular_kd 1.6
#define VEL_P_DIST 1.4
#define DIST_P 5.0
#define DIST_MIN 12.0
#define ERRO_ANG_OK 45

// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------

// ------------------ FUNÇÕES -------------------



/* ------------------ FUNCAO PEGARBLOCO -------------------

    Entrada: Estado atual na máquina de estados e refrência 
    para objeto que contém informações do robô
    Saida: 1 ou 0. 1 indica fim do processo
    Finalidade: Fazer com que o robô consiga pegar o bloco
    correspondente ao lugar que está (porto/plataforma)

----------------------------------------------------------*/

int pegarBloco(int *estado_atual, Robo *barco, bool tem_bloco, bool agarrado){

	

	    // troca entre estados de mais alto nivel
    switch(*estado_atual){

    

    	case 10: // estado IDENTIFICAR BLOCOS
    		

            *estado_atual = 11;
            // acho que isso é só ler do topico de visao o tempo todo e guardar no objeto barco
    		break;

    	case 11: // estado ESCOLHER BLOCO

            *estado_atual = 12;
    		// pega o array de blocos e mantem track da posicao do melhor deles
    		break;

    	case 12: // estado ESTENDER BRACO

    		if (tem_bloco)
                *estado_atual = 13;
    		break;

    	case 13: // estado AGARRAR

            // tem que fazer alguma coisa pra manter o motor funcionando o tempo necessario
            if (agarrado)
                *estado_atual = 14;
    		break;

    	case 14: // estado RECOLHER

    		//if (braco_retornou) // provavel que isso seja fornecido pela visao, quando a garra estiver a uma certa distancia
                *estado_atual = 15;
            break;

    	case 15: // estado GUARDAR

    		//if (braco_guardado) // sera fornecido por um sensor fim de curso
                *estado_atual = 16;
    		break;

    	case 16: // estado CONTAR

            if (barco->getLadoArena())
    		    barco->addBlocoAmarelo();
            else
                barco->addBlocoVermelho();

            return 1;

    		break;

    	default: 

    		break;



    }

    return 0;

}

/* ------------------ FUNCAO COLOCARBLOCO -------------------

    Entrada: Estado atual na máquina de estados e refrência 
    para objeto que contém informações do robô
    Saida: 1 ou 0. 1 indica fim do processo
    Finalidade: Fazer o robô se mover entre plataforma/porto

----------------------------------------------------------*/

int colocarBloco(int *estado_atual, Robo *barco){


	    // troca entre estados de mais alto nivel
    switch(*estado_atual){

    	case 30: // estado ESTENDER BRACO
    	
            
                        // publicar velocidade dos motores do braço para extender o braço até a garra alcançar algum bloco
    		break;

    	case 31: // estado DEIXAR BLOCO

    		// publicar velocidade do motor da garra para largar o bloc, talvez não seja necessário um estado só para isso
    		break;

    	case 32: // estado RECOLHER

    		// publicar velocidade dos motores do braço para recolher o braço até a garra estar no casco de volta
    		break;

    	case 33: // estado CONTAR


            if (barco->getLadoArena() == 1) // plataforma
                barco->addBlocoAmarelo();
            else if (barco->getLadoArena() == -1) // porto
                barco->addBlocoVermelho();
    		
    		break;

    	case 34: // estado PEGAR SEGUNDO BLOCO

    		// por enquanto nao sera implementado, primeiro deve-se conseguir pegar um bloco
    		break;


    	default: 

    		break;



    }


}



/* ------------------ FUNCAO TRANSPORTARBLOCO -------------------

    Entrada: Estado atual na máquina de estados e refrência 
    para objeto que contém informações do robô
    Saida: 1 ou 0. 1 indica fim do processo
    Finalidade: Fazer o robô se mover entre plataforma/porto

----------------------------------------------------------*/

int transportarBloco(int *estado_atual, Robo *barco, geometry_msgs::Pose2D *posicao_objetivo){

	

	    // troca entre estados de mais alto nivel
    switch(*estado_atual){

    	case 20: // estado DESATRACAR
    		
            if (desatracar(barco, posicao_objetivo))
               *estado_atual = 21;

    		break;

    	case 21: // estado LOCALIZA OBJETIVO (Porto ou plataforma)

    		// por enquanto nada será implementado. 
            *estado_atual = 22; // vai direto para o próximo estado
    		break;

    	case 22: // estado DECIDE ONDE GUARDAR

    		//defineObjetivo(barco);
            *estado_atual = 23; 
    		break;

        case 23: // estado DECIDE ONDE GUARDAR

            if (fazTrajetoria(barco, posicao_objetivo))
                *estado_atual = 24; 
            break;


    	case 24: // estado ATRACAR

    		if (atracar(barco, posicao_objetivo))
               return 1; // processo concluído
    		break;


    	default: 

    		break;


    }

    return 0; // processo ainda em andamento

}



