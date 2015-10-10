#include <stdio.h>



#include "estados.h"
#include "transporte.h"






void pegarBloco(int estado_atual){

	

	    // troca entre estados de mais alto nivel
    switch(estado_atual){

    

    	case 0: // estado IDENTIFICAR BLOCOS
    		

    		break;

    	case 1: // estado ESCOLHER BLOCO

    		
    		break;

    	case 2: // estado MOVER BRACO

    		
    		break;

    	case 3: // estado AGARRAR

    		
    		break;

    	case 4: // estado RECOLHER

    		
    		break;

    	case 5: // estado GUARDAR

    		
    		break;

    	case 6: // estado CONTAR

    		
    		break;

    	default: 

    		break;



    }

}

void colocarBloco(int estado_atual){


	    // troca entre estados de mais alto nivel
    switch(estado_atual){

    	case 0: // estado EXTENDER BRACO
    		


    		break;

    	case 1: // estado DEIXAR BLOCO

    		
    		break;

    	case 2: // estado RECOLHER

    		
    		break;

    	case 3: // estado CONTAR

    		
    		break;

    	case 4: // estado PEGAR SEGUNDO BLOCO

    		
    		break;


    	default: 

    		break;



    }


}

void transportarBloco(int *estado_atual, Robo barco){

	

	    // troca entre estados de mais alto nivel
    switch(*estado_atual){

    	case 20: // estado DESATRACAR
    		
            //if (desatracar(barco))
             //   *estado_atual = 21;

    		break;

    	case 21: // estado LOCALIZA OBJETIVO (Porto ou plataforma)

    		
    		break;

    	case 22: // estado DECIDE ONDE GUARDAR

    		
    		break;

    	case 23: // estado ATRACAR

    		
    		break;


    	default: 

    		break;



    }

}