/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
(31) 9405-5012
gustdomar@gmail.com

Código principal do pacote de estratéiga
*/



// -------------------- BIBLIOTECAS --------------------

#include <stdio.h>
#include <stdlib.h>


// ---------------- ARQUIVOS INCLUSOS ------------------





// -------------------- CONSTANTES --------------------



// ----------------- VARIÁVEIS GLOBAIS ------------------

float yaw;


// ---------------- CALLBACK FUNCTIONs -----------------

 void imu(const std_msgs::Float32& msg){

    yaw = msg.data;
  
 }
 




// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------


	// init ROS stuff
	ros::init(argc, argv, "normalizaYaw");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // ------------------------- SUBSCRIBERS -------------------------
    ros::Subscriber subIMU = nh.subscribe("eletronica/imu/yaw", 1000, imu);


    // ------------------------- PUBLISHERS -------------------------
  
    ros::Publisher pubYawNormalizado = nh.advertise <std_msgs::Int32>("estrategia/yawNormalizado", 1000);


    ros::Rate rate(2); // Hz


    // variaveis auxiliar para guardar o que sera publicado
    std_msgs::Int32 msg_yawNormalizado;



    // ----------------- INICIALIZAÇÔES ---------------------



	// ------------------------------------------------------



    

    if(argc < 3){
        ROS_INFO("\n\nUse: rosrun estrategia normalizaYaw [Angulo Plataforma]\n\n");
        return -1;
    }

    // parametros recebidos na chamada do nó
    float ang_plataforma = atof(argv[1]);


    while (ros::ok()){


    	

    	// preenchendo o que sera publicado
		msg_estado.data = 360.0 + yaw - ang_plataforma;
		msg_estado.data %= 360;

		if (msg_estado.data > 180)
			msg_estado.data -= 360;


		 // Publish the message 
		 pubYawNormalizado.publish(msg_yawNormalizado);

		 // Wait untilit's time for another iteration.
		 rate.sleep();
		 ros::spinOnce();
 	}



}