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



#include "robo.h"
#include "estados.h"
#include "transporte.h"


// -------------------- CONSTANTES --------------------



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

int pegarBloco(int estado_atual, Robo *barco){

	

	    // troca entre estados de mais alto nivel
    switch(estado_atual){

    

    	case 10: // estado IDENTIFICAR BLOCOS
    		

    		break;

    	case 11: // estado ESCOLHER BLOCO

    		
    		break;

    	case 12: // estado MOVER BRACO

    		
    		break;

    	case 13: // estado AGARRAR

    		
    		break;

    	case 14: // estado RECOLHER

    		
    		break;

    	case 15: // estado GUARDAR

    		
    		break;

    	case 16: // estado CONTAR

    		
    		break;

    	default: 

    		break;



    }

}

/* ------------------ FUNCAO COLOCARBLOCO -------------------

    Entrada: Estado atual na máquina de estados e refrência 
    para objeto que contém informações do robô
    Saida: 1 ou 0. 1 indica fim do processo
    Finalidade: Fazer o robô se mover entre plataforma/porto

----------------------------------------------------------*/

int colocarBloco(int estado_atual, Robo *barco){


	    // troca entre estados de mais alto nivel
    switch(estado_atual){

    	case 30: // estado EXTENDER BRACO
    		


    		break;

    	case 31: // estado DEIXAR BLOCO

    		
    		break;

    	case 32: // estado RECOLHER

    		
    		break;

    	case 33: // estado CONTAR


            if (barco->getLadoArena() == 1) // plataforma
                barco->addBlocoAmarelo();
            else if (barco->getLadoArena() == -1) // porto
                barco->addBlocoVermelho();
    		
    		break;

    	case 34: // estado PEGAR SEGUNDO BLOCO

    		
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

    	case 23: // estado ATRACAR

    		if (atracar(barco))
               return 1; // processo concluído
    		break;


    	default: 

    		break;



    }

    return 0; // processo ainda em andamento

}