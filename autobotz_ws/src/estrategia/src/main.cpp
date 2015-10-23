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
bool atracado;
bool tem_bloco;
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

 void atracadoMsgRecieved (const std_msgs::Int32 msg){

    atracado = msg.data;

 }

void temBlocoMsgRecieved (const std_msgs::Bool msg){

    tem_bloco = msg.data;

 }


 //void temBlocoMsgRecieved (const geometry_msgs::Pose2D& msg){

    //tem_bloco[0] = ;
    //tem_bloco[1] = ;

 //}


// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	int estado_atual;
	int id_controlador=0;


	// init ROS stuff
	ros::init(argc, argv, "estrategia");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // ------------------------- SUBSCRIBERS -------------------------
    ros::Subscriber subInicio = nh.subscribe("eletronica/start", 1000, &inicioMsgRecieved);
    ros::Subscriber subPosicao = nh.subscribe("estrategia/barco/posicao", 1000, &posicaoMsgRecieved);
    ros::Subscriber subAtracado = nh.subscribe("controle/barco/atracado", 1000, &atracadoMsgRecieved);
    ros::Subscriber subTemBloco = nh.subscribe("/eletronica/garra/temBloco", 1000, &temBlocoMsgRecieved);
/eletronica/garra/temBloco

    // ------------------------- PUBLISHERS -------------------------
    ros::Publisher pub_estado = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Publisher pub_trajetoria = nh.advertise <estrategia::trajetoria>("estrategia/trajetoria", 1000);
    ros::Publisher pub_velocidade = nh.advertise <estrategia::velocidade>("estrategia/velocidade", 1000);
    ros::Publisher pub_controlador = nh.advertise <estrategia::velocidade>("estrategia/id_controlador", 1000);





    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_estado; 
    std_msgs::Int32 msg_controlador; 
    estrategia::trajetoria msg_trajetoria;
    estrategia::velocidade msg_velocidade;

    // PROVISORIO
//    std_msgs::Int32 msg_propulsorR, msg_propulsorL;


    while (ros::ok()){


    	if (start){

		    // troca entre estados de mais alto nivel
		    switch(estado_atual){

		    	case 0: // estado INCIAL
		    		
		    		
		    		estado_atual = 10;
		    		id_controlador = 0;

		    		estado_atual = 0;
					start = false;

 
					tem_bloco[0] = 0;
					tem_bloco[1] = 0;
					barco.zeraAtributos();

		    		break;

		    	case 10: // estado PEGAR BLOCO
		    	case 11:
		    	case 12:
		    	case 13:
		    	case 14:
		    	case 15:
		    	case 16:

		    		id_controlador = 0;
		    		//pegarBloco(estado_atual, array_de_blocos_visiveis, tem_bloco[2]);
		    		//if (pegarBloco(&estado_atual, &barco, tem_bloco))
		    		if (atracado)
		    			estado_atual = 20;

		    		sleep(TEMPO_ATRACAR);

		    		// velocidade angular zero e linear o suficiente para manter o barco atracado
		    		//barco.setVelocidadeBarco(VEL_ATRACAR, 0);
		    		break;

		    	case 20: // estado TRANSPORTE
		    	case 21:
		    	case 22:
		    	case 23:
		    	case 24:

		    		id_controlador = 2; // controlador por angulo e distancia
		    		//if (transportarBloco(&estado_atual, &barco, &posicao_objetivo))
		    		if (atracado)
		    			estado_atual = 30;
		    		break;

		    	case 30: // estado DEIXAR BLOCO

		    		id_controlador = 0;

		    		//colocarBloco(estado_atual);
		    		if (barco.getBlocosVermelhos() >= TOTAL_BLOCOS_VERMELHOS &&
		    		    barco.getBlocosAmarelos() >= TOTAL_BLOCOS_AMARELOS)

		    			estado_atual = 40;

		    		else

		    			estado_atual = 10;
		    		break;


		    	case 40: // FIM

		    		// tem que apertar o botao desligar para o barco começar de novo
		    		if (!start)
		    			estado_atual = 0;
		    		break;

		    	default: 

		    		break;


		    }



		}

		else {
			id_controlador = 0;
		   	barco.zeraAtributos();
		   	estado_atual = 0;
		}


    	// preenchendo o que sera publicado
		msg_estado.data = estado_atual;
		msg_controlador.data = id_controlador;
		msg_trajetoria = barco.getTrajetoria();
		msg_velocidade = barco.getVelocidadeBarco();


		 // Publish the message 
		 pub_estado.publish(msg_estado);
		 pub_controlador.publish(msg_controlador);
		 pub_trajetoria.publish(msg_trajetoria);
		 pub_velocidade.publish(msg_velocidade);


		 // Wait untilit's time for another iteration.
		 rate.sleep();
		 ros::spinOnce();
 	}



}