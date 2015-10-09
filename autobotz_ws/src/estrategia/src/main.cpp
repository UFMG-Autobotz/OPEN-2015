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




// ---------------- ARQUIVOS INCLUSOS ------------------

#include "estados.h"


// -------------------- CONSTANTES --------------------



// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;


// ---------------- CALLBACK FUNCTIONs -----------------

void poseMessageReceived (const std_msgs::Bool& msg){

    start = msg.data;

 }




// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------


	std_msgs::Int32 estado_atual;
	estado_atual.data = 0;
	start = false;




	// init ROS stuff
	ros::init(argc, argv, "estrategia");

	// Establish this program as a ROS node
    ros::NodeHandle nh;



    // troca entre estados de mais alto nivel
    switch(estado_atual.data){

    	case 0: // estado INCIAL
    		if (start)
    			estado_atual.data++;
    		break;

    	case 1: // estado PEGAR BLOCO

    		pegarBloco();
    		break;

    	case 2: // estado TRANSPORTE

    		transportarBloco();
    		break;

    	case 3: // estado DEIXAR BLOCO

    		colocarBloco();
    		break;


    	case 4: // FIM

    		// tem que apertar o botao desligar o barco para começar de novo
    		if (!start)
    			estado_atual.data = 0;
    		break;

    	default: 

    		break;



    }


    ros::Subscriber sub = nh.subscribe("visao/squares/centers", 1000, &poseMessageReceived);

    ros::Publisher pub = nh.advertise <std_msgs::Int32>("estrategia/estado_atual", 1000);
    ros::Rate rate(2); // Hz

    while (ros::ok()){

    
		 std_msgs::Int32 msg ;
		 msg.data = estado_atual.data;

		 // Pub lish the message .
		 pub.publish(msg);

		 // Wait un t i l i t ' s time for another i t e ra t ion .
		 rate.sleep();
		 ros::spinOnce();
 	}



}