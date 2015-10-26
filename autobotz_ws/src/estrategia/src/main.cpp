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

// -------------------- CONSTANTES --------------------

#define TOTAL_BLOCOS_VERMELHOS 4 // informacao retirada do edital 
#define TOTAL_BLOCOS_AMARELOS 10 // informacao retirada do edital 

#define VEL_ATRACAR 70
#define VEL_MAX 255
#define VEL_MIN -255

#define TEMPO_ATRACAR 1000 // em ms

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
bool atracado, agarrado, tem_bloco;
Robo barco; // assim não chama o construtor


geometry_msgs::Pose2D posicao_objetivo;
int blocos_vermelhos, blocos_amarelos;

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

 




// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	int estado_atual;



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
 

    // ------------------------- PUBLISHERS -------------------------
    ros::Publisher pub_estado = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Publisher pub_trajetoria = nh.advertise <estrategia::trajetoria>("estrategia/trajetoria", 1000);
    ros::Publisher pub_velocidade = nh.advertise <estrategia::velocidade>("estrategia/velocidade", 1000);
    





    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_estado; 
    estrategia::trajetoria msg_trajetoria;
    estrategia::velocidade msg_velocidade;

    // PROVISORIO
//    std_msgs::Int32 msg_propulsorR, msg_propulsorL;

	estado_atual = 0;
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

		    		

		    		// velocidade angular zero e linear o suficiente para manter o barco atracado
		    		//barco.setVelocidadeBarco(VEL_ATRACAR, 0);
		    		break;

		    	case 20: // estado TRANSPORTE
		    	case 21:
		    	case 22:
		    	case 23:
		    	case 24:

		    		//if (transportarBloco(&estado_atual, &barco, &posicao_objetivo))
		    		if (atracado)
		    			estado_atual = 30;

		    		sleep(TEMPO_ATRACAR);

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


		 // Publish the message 
		 pub_estado.publish(msg_estado);
		 pub_trajetoria.publish(msg_trajetoria);
		 pub_velocidade.publish(msg_velocidade);


		 // Wait untilit's time for another iteration.
		 rate.sleep();
		 ros::spinOnce();
 	}



}