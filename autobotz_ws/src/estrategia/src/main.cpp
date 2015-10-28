/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
(31) 9405-5012
gustdomar@gmail.com

Código principal do pacote de estratéiga
*/



// -------------------- BIBLIOTECAS --------------------

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Pose2D.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"
#include "estrategia/velocidade.h"
#include "estrategia/feature.h"
#include "estrategia/featureVec.h"
#include "./Funcoes/auxiliares.hpp"


// -------------------- CONSTANTES --------------------

#define TELA_X 640
#define TELA_Y 480

#define TOTAL_BLOCOS_VERMELHOS 4 // informacao retirada do edital 
#define TOTAL_BLOCOS_AMARELOS 10 // informacao retirada do edital 

#define VEL_ATRACAR 70
#define VEL_MAX 255
#define VEL_MIN -255

#define TEMPO_ALCANCA_BLOCO 1000 // ms
#define TEMPO_AGARRA_BLOCO 3000 // ms
#define TEMPO_ESTENDE_BRACO 3000 // ms
#define TEMPO_SOLTAR_BLOCO 3000 // ms
#define TEMPO_GUARDA_BRACO 5000 // ms
#define TEMPO_GUARDA_BLOCO 5000 // ms


#define TEMPO_ATRACAR 1000 // em ms
#define DIST_ATRACAR 15 // em cm

#define ERRO_ANG_MORTO 3

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
bool atracado, agarrado, tem_bloco;
int blocos_vermelhos, blocos_amarelos;
float distL, distR, distF, distB, destino_x;


Robo barco; // assim não chama o construtor
geometry_msgs::Pose2D posicao_objetivo;
estrategia::featureVec blocos;


// ---------------- CALLBACK FUNCTIONs -----------------

void inicioMsgRecieved (const std_msgs::Bool& msg){

    start = msg.data;

 }

void posicaoMsgRecieved (const geometry_msgs::Pose2D& msg){

    barco.setPosicao(msg.x, msg.y, msg.theta);

 }

 

  void temBlocoMsgRecieved (const std_msgs::Bool msg){

    tem_bloco = msg.data;

 }


void ultrassomL (const std_msgs::Float32& msg){

    distL = msg.data;

 }

void ultrassomR (const std_msgs::Float32& msg){

    distR = msg.data;

 }

 void ultrassomF (const std_msgs::Float32& msg){

    distF = msg.data;

 }

 void ultrassomB (const std_msgs::Float32& msg){

    distB = msg.data;

 }

 void blocosMsgRecieved (const estrategia::featureVec msg){


    blocos = msg;

    int i;
    printf ("\n\n-------------- LENDO VISAO ---------------\n\n");
    printf ("Numero de blobs: %li\n", blocos.features.size());
    for (i=0; i<blocos.features.size(); i++)
    	printf ("#%i - pos: %lf ", i, blocos.features[i].centroid.x);
    printf ("\nDestino: %.2f\n", destino_x);
    printf ("\n\n------------------- FIM ------------------\n\n");

 }

 




// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	int estado_atual;
	int lado_arena;
	float angulo_saida;
	bool atracado;

	std::vector<float> blocos_anteriores;
	float bloco_objetivo_X;



	// init ROS stuff
	ros::init(argc, argv, "estrategia");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // ------------------------- SUBSCRIBERS -------------------------
    ros::Subscriber subInicio = nh.subscribe("eletronica/start", 1000, &inicioMsgRecieved);
    ros::Subscriber subPosicao = nh.subscribe("estrategia/barco/posicao", 1000, &posicaoMsgRecieved);
    ros::Subscriber subTemBloco = nh.subscribe("eletronica/garra/temBloco", 1000, &temBlocoMsgRecieved);
  

    ros::Subscriber subR = nh.subscribe("eletronica/ultrassom/R", 1000, ultrassomR);
    ros::Subscriber subL = nh.subscribe("eletronica/ultrassom/L", 1000, ultrassomL);
    ros::Subscriber subF = nh.subscribe("eletronica/ultrassom/F", 1000, ultrassomF);
    ros::Subscriber subB = nh.subscribe("eletronica/ultrassom/B", 1000, ultrassomB);
 
    ros::Subscriber subBlocos = nh.subscribe("/visao/features", 1000, &blocosMsgRecieved);


    // ------------------------- PUBLISHERS -------------------------
    ros::Publisher pubEstado = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Publisher pubTrajetoria = nh.advertise <estrategia::trajetoria>("estrategia/trajetoria", 1000);
    ros::Publisher pubVelocidade = nh.advertise <estrategia::velocidade>("estrategia/velocidade", 1000);

    ros::Publisher pubTransporteDestino = nh.advertise <std_msgs::Float32>("estrategia/transporte/destino", 1000);
    ros::Publisher pubTransporteAnguloSaida = nh.advertise <std_msgs::Float32>("estrategia/transporte/angulo_saida", 1000);
    ros::Publisher pubBlocoObjetivoX = nh.advertise <std_msgs::Float32>("estrategia/bloco/bloco_objetivo_x", 1000);
    
    


    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_estado; 
    estrategia::velocidade msg_velocidade;
    std_msgs::Float32 msg_destino, msg_anguloSaida;
    std_msgs::Float32 msg_blocoObjetivoX;



    // ----------------- INICIALIZAÇÔES ---------------------

	estado_atual = 0;
	lado_arena = -1; // -1 indica porto e 1 indica plataforma
	atracado = false;

	// ------------------------------------------------------


    while (ros::ok()){


    		printf ("Estado: %d\n", estado_atual);

		    // troca entre estados de mais alto nivel
		    switch(estado_atual){

		    	case 0: // estado INCIAL
		    		

					
 
					barco.zeraAtributos();

					estado_atual = 10;
					

		    		break;


		    	// estado PEGAR BLOCO
		    	case 10: // estado IDENTIFICAR BLOCOS

		    			

		    			//break;
		    	case 11: // estado ESCOLHER 

		    			//bloco_objetivo = escolherBloco(blocos);
		    			//estado_atual = 12;
		    			//break;
		    	case 12: // estado ESTENDER BRAÇO

		    			bloco_objetivo_X = trackBloco(blocos);

		    			if (tem_bloco)
		    				estado_atual = 13;


		    			msg_blocoObjetivoX.data = destino_x;

		    			break;

		    	case 13: // estado AGARRAR
		    		    // espera um tempo para a garra alcançar melhor o bloco
                		sleep(TEMPO_ALCANCA_BLOCO);
                		estado_atual = 14;
                		break;

		    	case 14: // estado RECOLHER
		                // espera um tempo para a garra pegar melhor o bloco
		                sleep(TEMPO_AGARRA_BLOCO);
		                estado_atual = 15;
		    			break;

		    	case 15: //estado GUARDAR

		    			sleep(TEMPO_GUARDA_BLOCO);
		    			estado_atual = 16;

		    	case 16: // estado CONTAR

		    		
		    		//pegarBloco(estado_atual, array_de_blocos_visiveis, tem_bloco);
		    		//if (pegarBloco(&estado_atual, &barco, tem_bloco, agarrado))		    		
		    			estado_atual = 20;


		    			if (abs(barco.getPosicao().theta) < 90.0) // se barco esta voltado para a plataforma
		    				lado_arena = 1; // ele esta na plataforma
		    			else // porto
		    				lado_arena = -1;

		    		break;



//######################################################################################################################

		    	// estado TRANSPORTE

		    	case 20: // DESATRACAR

		    		if (lado_arena == -1) // barco esta no porto
		    			angulo_saida = 0.0; // considerando o angulo da imu normalizado e o 0 paralelo a borda da piscina
		    		else if (lado_arena == 1) // barco esta na plataforma
		    			angulo_saida = 180.0;

		    		if (abs(barco.getPosicao().theta - angulo_saida) <= ERRO_ANG_MORTO)
		    			estado_atual = 21;


		    		//estado_atual = 21; // so pra teste
		    		break;

		    	case 21: // LOCALIZA destino e NAVEGA!!

		    		destino_x = localizaDestino(blocos, &blocos_anteriores);
		    		
		    	
		    		if (atracado){
		    			estado_atual = 22;
		    			sleep(TEMPO_ATRACAR);
		    		}

		    		break;

		    	case 22:


		    		if (lado_arena == -1) // atracou na plataforma
		    			estado_atual = 30; // nao pega bloco e volta pro porto
		    		else if (lado_arena = 1) // atracou no porto
		    				estado_atual = 10; // pega novo bloco

		    		break;


//##################################################################################################################

		    	// estado DEIXAR BLOCO

		    	case 30: // acha LUGAR DISPONIVEL 

		    			// funcao que retorna um agulo bom para lancar o braco
		    			//estado_atual = 31;
		    			//break;

		    	case 31: // estado ESTENDER BRAÇO

		    						
		    			estado_atual = 32;

		    			break;

		    	case 32: // estado SOLTAR bloco

		    		    // espera um tempo para o braço estender o suficiente
                		sleep(TEMPO_ESTENDE_BRACO);
                		estado_atual = 33;
                		break;

		    	case 33: // estado RECOLHER
		                // espera um tempo para a garra pegar melhor o bloco
		                sleep(TEMPO_SOLTAR_BLOCO);
		                estado_atual = 34;
		    			break;

		    	case 34: //estado GUARDAR

		    			// if (recolhido);
		    			//	estado_atual = 35;
		    			
		    			break;

		    	case 35: // estado CONTAR

		    		
		    		


		    			if (abs(barco.getPosicao().theta) < 90.0) // se barco esta voltado para a plataforma
		    				lado_arena = 1; // ele esta na plataforma
		    			else // porto
		    				lado_arena = -1;

		    		break;

		    		
		    		//colocarBloco(estado_atual);
		    		if (barco.getBlocosVermelhos() >= TOTAL_BLOCOS_VERMELHOS &&
		    		    barco.getBlocosAmarelos() >= TOTAL_BLOCOS_AMARELOS)

		    			estado_atual = 40;

		    		else{

		    			estado_atual = 20; 

		    		}
		    		break;


		    	case 40: // FIM

		    		
		    	   		break;

		    	default: 

		    		break;


		    }



		 if (distF < DIST_ATRACAR)
		 	atracado = true;

		
		// tem que apertar o botao desligar para o barco começar de novo
		/*if (!start) {
			
		   	barco.zeraAtributos();
		   	estado_atual = 0;
		}*/


    	// preenchendo o que sera publicado
		msg_estado.data = estado_atual;
		
		msg_velocidade = barco.getVelocidadeBarco();
		msg_destino.data = destino_x;
		msg_anguloSaida.data = angulo_saida;


		 // Publish the message 
		 pubEstado.publish(msg_estado);
		 pubBlocoObjetivoX.publish(msg_blocoObjetivoX);
		 pubVelocidade.publish(msg_velocidade);
		 pubTransporteDestino.publish(msg_destino);
		 pubTransporteAnguloSaida.publish(msg_anguloSaida);

		 // Wait untilit's time for another iteration.
		 rate.sleep();
		 ros::spinOnce();
 	}



}