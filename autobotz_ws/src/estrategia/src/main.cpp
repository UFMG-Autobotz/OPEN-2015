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


// ---------------- ARQUIVOS INCLUSOS ------------------


#include "robo.h"
#include "estados.h"
#include "estrategia/trajetoria.h"
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

#define TEMPO_ATRACAR 1000 // em ms

#define ERRO_ANG_MORTO 3

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
bool atracado, agarrado, tem_bloco;
int blocos_vermelhos, blocos_amarelos;


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

 void atracadoMsgRecieved (const std_msgs::Bool msg){

    atracado = msg.data;

 }

  void temBlocoMsgRecieved (const std_msgs::Bool msg){

    tem_bloco = msg.data;

 }

 void agarradoMsgRecieved (const std_msgs::Int32 msg){

    agarrado = msg.data;

 }


 void blocosMsgRecieved (const estrategia::featureVec msg){

 	int i;
    blocos = msg;

    printf ("\n\n-------------- LENDO VISAO ---------------\n\n");
    printf ("Numero de blobs: %li\n", blocos.features.size());
    for (i=0; i<blocos.features.size(); i++)
    	printf ("#%i - pos: %lf", i, blocos.features[i].centroid.x);
    printf ("\n\n------------------- FIM ------------------\n\n");

 }

 




// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	int estado_atual;
	int lado_arena;
	float angulo_saida, destino_x;


	// init ROS stuff
	ros::init(argc, argv, "estrategia");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // ------------------------- SUBSCRIBERS -------------------------
    ros::Subscriber subInicio = nh.subscribe("eletronica/start", 1000, &inicioMsgRecieved);
    ros::Subscriber subPosicao = nh.subscribe("estrategia/barco/posicao", 1000, &posicaoMsgRecieved);
    ros::Subscriber subAtracado = nh.subscribe("controle/barco/atracado", 1000, &atracadoMsgRecieved);
    ros::Subscriber subTemBloco = nh.subscribe("eletronica/garra/temBloco", 1000, &temBlocoMsgRecieved);
    ros::Subscriber subAgarrado = nh.subscribe("controle/barco/agarrado", 1000, &agarradoMsgRecieved);
 
    ros::Subscriber subBlocos = nh.subscribe("/visao/features", 1000, &blocosMsgRecieved);


    // ------------------------- PUBLISHERS -------------------------
    ros::Publisher pubEstado = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Publisher pubTrajetoria = nh.advertise <estrategia::trajetoria>("estrategia/trajetoria", 1000);
    ros::Publisher pubVelocidade = nh.advertise <estrategia::velocidade>("estrategia/velocidade", 1000);

    ros::Publisher pubTransporteDestino = nh.advertise <std_msgs::Float32>("estrategia/transporte/destino", 1000);
    ros::Publisher pubTransporteAnguloSaida = nh.advertise <std_msgs::Float32>("estrategia/transporte/angulo_saida", 1000);
    


    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_estado; 
    estrategia::trajetoria msg_trajetoria;
    estrategia::velocidade msg_velocidade;
    std_msgs::Float32 msg_destino, msg_anguloSaida;



    // ----------------- INICIALIZAÇÔES ---------------------

	estado_atual = 0;
	lado_arena = -1; // -1 indica porto e 1 indica plataforma

	// ------------------------------------------------------


    while (ros::ok()){


    		printf ("Estado: %d\n", estado_atual);

		    // troca entre estados de mais alto nivel
		    switch(estado_atual){

		    	case 0: // estado INCIAL
		    		

					start = false;

 
					barco.zeraAtributos();

					//if (start){

					estado_atual = 10;
					//}

		    		break;

		    	case 10: // estado PEGAR BLOCO
		    	case 11:
		    	case 12:
		    	case 13:
		    	case 14:
		    	case 15:
		    	case 16:

		    		
		    		//pegarBloco(estado_atual, array_de_blocos_visiveis, tem_bloco);
		    		//if (pegarBloco(&estado_atual, &barco, tem_bloco, agarrado))		    		
		    			estado_atual = 20;


		    			if (abs(barco.getPosicao().theta) < 90.0) // se barco esta voltado para a plataforma
		    				lado_arena = 1; // ele esta na plataforma
		    			else // porto
		    				lado_arena = -1;

		    		// velocidade angular zero e linear o suficiente para manter o barco atracado
		    		//barco.setVelocidadeBarco(VEL_ATRACAR, 0);
		    		break;

		    	// estado TRANSPORTE

		    	case 20: // DESATRACAR

		    		if (lado_arena == -1) // barco esta no porto
		    			angulo_saida = 0.0; // considerando o angulo da imu normalizado e o 0 paralelo a borda da piscina
		    		else if (lado_arena == 1) // barco esta na plataforma
		    			angulo_saida = 180.0;

		    		if (abs(barco.getPosicao().theta - angulo_saida) <= ERRO_ANG_MORTO)
		    			estado_atual = 21;


		    		estado_atual = 21;
		    		break;

		    	case 21: // LOCALIZA destino

		    		destino_x = localizaDestino(blocos);
		    		
		    		if (destino_x > 0)
		    			estado_atual = 22;
		    	case 22: // NAVEGAR

		    		//transportarBloco(&estado_atual, lado_arena, &barco, blocos);
		    		if (atracado){
		    			estado_atual = 30;
		    			sleep(TEMPO_ATRACAR);
		    		}

		    		

		    		break;

		    	case 30: // estado DEIXAR BLOCO

		    		
		    		//colocarBloco(estado_atual);
		    		if (barco.getBlocosVermelhos() >= TOTAL_BLOCOS_VERMELHOS &&
		    		    barco.getBlocosAmarelos() >= TOTAL_BLOCOS_AMARELOS)

		    			estado_atual = 40;

		    		else

		    			estado_atual = 10;
		    		break;


		    	case 40: // FIM

		    		
		    	   		break;

		    	default: 

		    		break;


		    }



		
		// tem que apertar o botao desligar para o barco começar de novo
		/*if (!start) {
			
		   	barco.zeraAtributos();
		   	estado_atual = 0;
		}*/


    	// preenchendo o que sera publicado
		msg_estado.data = estado_atual;
		msg_trajetoria = barco.getTrajetoria();
		msg_velocidade = barco.getVelocidadeBarco();
		msg_destino.data = destino_x;
		msg_anguloSaida.data = angulo_saida;


		 // Publish the message 
		 pubEstado.publish(msg_estado);
		 pubTrajetoria.publish(msg_trajetoria);
		 pubVelocidade.publish(msg_velocidade);
		 pubTransporteDestino.publish(msg_destino);
		 pubTransporteAnguloSaida.publish(msg_anguloSaida);

		 // Wait untilit's time for another iteration.
		 rate.sleep();
		 ros::spinOnce();
 	}



}