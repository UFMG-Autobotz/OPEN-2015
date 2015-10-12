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

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
Robo barco;

int blocos_vermelhos, blocos_amarelos;

// ---------------- CALLBACK FUNCTIONs -----------------

void inicioMsgRecieved (const std_msgs::Bool& msg){

    start = msg.data;

 }

void posicaoMsgRecieved (const geometry_msgs::Pose2D& msg){

    barco.setPosicao(msg.x, msg.y, msg.theta);

 }


// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


// ------------ VARIAVEIS ------------


	int estado_atual;
	estado_atual = 0;
	start = false;

    
	// TESTES
	barco.setObjetivo(370.0, 150.0, 180.0);


	// init ROS stuff
	ros::init(argc, argv, "estrategia");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // ------------------------- SUBSCRIBERS -------------------------
    ros::Subscriber subInicio = nh.subscribe("eletronica/start", 1000, &inicioMsgRecieved);
    ros::Subscriber subPosicao = nh.subscribe("estrategia/barco/posicao", 1000, &posicaoMsgRecieved);



    // ------------------------- PUBLISHERS -------------------------
    ros::Publisher pub_estado = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Publisher pub_trajetoria = nh.advertise <estrategia::trajetoria>("estrategia/trajetoria", 1000);
    ros::Publisher pub_velocidade = nh.advertise <estrategia::velocidade>("estrategia/velocidade", 1000);




    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_estado; 
    estrategia::trajetoria msg_trajetoria;
    estrategia::velocidade msg_velocidade;


    while (ros::ok()){


	    // troca entre estados de mais alto nivel
	    switch(estado_atual){

	    	case 0: // estado INCIAL
	    		if (start)
	    			estado_atual = 10;
	    		break;

	    	case 10: // estado PEGAR BLOCO

	    		//pegarBloco(estado_atual);
	    		estado_atual = 20;
	    		break;

	    	case 20: // estado TRANSPORTE
	    	case 21:

	    		if (transportarBloco(&estado_atual, &barco))
	    			estado_atual = 30;
	    		break;

	    	case 30: // estado DEIXAR BLOCO

	    		//colocarBloco(estado_atual);
	    		if (barco.getBlocosVermelhos() >= TOTAL_BLOCOS_VERMELHOS &&
	    		    barco.getBlocosAmarelos() >= TOTAL_BLOCOS_AMARELOS)

	    			estado_atual = 40;
	    		break;


	    	case 40: // FIM

	    		// tem que apertar o botao desligar para o barco começar de novo
	    		if (!start)
	    			estado_atual = 0;
	    		break;

	    	default: 

	    		break;


	    }


    	// preenchendo o que sera publicado
		msg_estado.data = estado_atual;
		msg_trajetoria = barco.getTrajetoria();
		msg_velocidade = barco.getVelocidade();

		 // Publish the message 
		 pub_estado.publish(msg_estado);
		 pub_trajetoria.publish(msg_trajetoria);
		 pub_velocidade.publish(msg_velocidade);

		 // Wait un t i l i t ' s time for another i t e ra t ion .
		 rate.sleep();
		 ros::spinOnce();
 	}



}