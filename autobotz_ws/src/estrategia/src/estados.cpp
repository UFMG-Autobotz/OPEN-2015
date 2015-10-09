#include <stdio.h>


void pegarBloco(){

	std_msgs::Int32 estado_atual;
	estado_atual.data = 0;

	    // troca entre estados de mais alto nivel
    switch(estado_atual.data){

    

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

void colocarBloco(){

	std_msgs::Int32 estado_atual;
	estado_atual.data = 0;

	    // troca entre estados de mais alto nivel
    switch(estado_atual.data){

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

void transportarBloco(){

	std_msgs::Int32 estado_atual;
	estado_atual.data = 0;
	    // troca entre estados de mais alto nivel
    switch(estado_atual.data){

    	case 0: // estado DESATRACAR
    		

    		break;

    	case 1: // estado LOCALIZA OBJETIVO (Porto ou plataforma)

    		
    		break;

    	case 2: // estado DECIDE ONDE GUARDAR

    		
    		break;

    	case 3: // estado ATRACAR

    		
    		break;


    	default: 

    		break;



    }

}