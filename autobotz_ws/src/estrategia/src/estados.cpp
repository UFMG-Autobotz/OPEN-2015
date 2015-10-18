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

// ---------------- ARQUIVOS INCLUSOS ------------------

#ifndef INCLUIR
#define INCLUIR

#include "robo.h"
#include "estados.h"
#include "transportarBloco.h"
#include "pegarBloco.h"


#endif

// -------------------- CONSTANTES --------------------

#define ANG_RECOLHIMENTO 60.0

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

int pegarBloco(int *estado_atual, Robo *barco, int tem_bloco[2]){

	

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

    		if (estenderBraco(barco, tem_bloco))
                *estado_atual = 13;
    		break;

    	case 13: // estado AGARRAR

            // tem que fazer alguma coisa pra manter o motor funcionando o tempo necessario
            if (agarrarBloco(barco))
                *estado_atual = 14;
    		break;

    	case 14: // estado RECOLHER

    		if (recolherBraco(barco)) // precisa saber de onde vira informacao da hora de parar
                *estado_atual = 15;
            break;

    	case 15: // estado GUARDAR

    		if (guardarBraco(barco, ANG_RECOLHIMENTO)) 
                *estado_atual = 16;
            break;
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

int transportarBloco(int *estado_atual, Robo *barco){

	

	    // troca entre estados de mais alto nivel
    switch(*estado_atual){

    	case 20: // estado DESATRACAR
    		
            if (desatracar(barco))
               *estado_atual = 21;

    		break;

    	case 21: // estado LOCALIZA OBJETIVO (Porto ou plataforma)

    		// por enquanto nada será implementado. 
            *estado_atual = 22; // vai direto para o próximo estado
    		break;

    	case 22: // estado DECIDE ONDE GUARDAR

    		defineObjetivo(barco);
            *estado_atual = 23; 
    		break;

        case 23: // estado DECIDE ONDE GUARDAR

            if (fazTrajetoria(barco))
                *estado_atual = 24; 
            break;


    	case 24: // estado ATRACAR

    		if (atracar(barco))
               return 1; // processo concluído
    		break;


    	default: 

    		break;


    }

    return 0; // processo ainda em andamento

}