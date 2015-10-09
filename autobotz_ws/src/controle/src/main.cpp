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
#include <std_msgs/Float32.h>
#include <geometry_msgs/Point.h>
#include <stdio.h>



// ---------------- ARQUIVOS INCLUSOS ------------------

#include "controle/squareCenters.h" //custom message

// -------------------- CONSTANTES --------------------

#define TELA_LIN 480
#define TELA_COL 640
#define VEL_MAX 510
#define VEL_MIN 0
#define VEL_NOR 255 // intervalo de 0 a 510 para ser convertido de -255 a 255. Portanto 305 -> 50
#define VEL_RET 400
#define VEL_P 1.4
#define DIST_P 5.0
#define DIST_MIN 12.0


#define MODO 2
#define ANG 180 // nao aceita numero negativo. Intervalo eh de 0 a 360 para ser convertido de -180 a 180. Portanto 180 -> 0

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
float distL, distR, distF, distB;
float yaw;
geometry_msgs::Point centerSquare;

// ---------------- CALLBACK FUNCTIONs -----------------

void poseMessageReceived (const std_msgs::Bool& msg){

    start = msg.data;

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


 void imu(const std_msgs::Float32& msg){

    yaw = msg.data;
    printf ("YAW: %.2f\n", yaw);
 }

 void cubosAmarelos(const controle::squareCenters::ConstPtr& msg)
{
    //a menssagem contem apenas um campo que se chama centers e é do tipo vector
    //ao acessar msg->centers, pode-se tratar centers como se fosse um
    //vector declarado com a sintaxe:
    // 
    //std::vector < geometry_msgs::Point > centers; 

    //system("clear");
    //ROS_INFO("Got array of size %i", int(msg->centers.size()));
    //ROS_INFO("Content:");
    //for(int i = 0; i < msg->centers.size(); i++)
    //{
        //ROS_INFO("point %i", i);
        //ROS_INFO("\tx: %i,y: %i", int(msg->centers[i].x), int(msg->centers[i].y));
    //}
    centerSquare = msg->centers[0];
    //ROS_INFO(" ");
}

// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	start = false;
    geometry_msgs::Point objetivo;
    float erro;



	// init ROS stuff
	ros::init(argc, argv, "controle");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    ros::Subscriber subR = nh.subscribe("eletronica/ultrassom/R", 1000, ultrassomR);
    ros::Subscriber subL = nh.subscribe("eletronica/ultrassom/L", 1000, ultrassomL);
    ros::Subscriber subF = nh.subscribe("eletronica/ultrassom/F", 1000, ultrassomF);
    ros::Subscriber subB = nh.subscribe("eletronica/ultrassom/B", 1000, ultrassomB);

    ros::Subscriber sub = nh.subscribe("visao/squares/centers", 1000, cubosAmarelos);

    ros::Subscriber subIMU = nh.subscribe("eletronica/imu/yaw", 1000, imu);


    ros::Publisher pubR = nh.advertise <std_msgs::Int32>("eletronica/propulsor/R", 1000);
    ros::Publisher pubL = nh.advertise <std_msgs::Int32>("eletronica/propulsor/L", 1000);
    ros::Rate rate(2); // Hz

    std_msgs::Int32 msg_propulsorR, msg_propulsorL;

    while (ros::ok()){

		 if (MODO == 1){  // segue a direcao do bloco

            // calcula quao desalinhado o bloco esta do robo
            erro = TELA_COL/2 - centerSquare.x;

        }

        else if(MODO == 2){ // segue um angulo desejado

            // calcula quao desalinhado o bloco esta do robo
            erro = (ANG - 180) - yaw; // ajusta o angulo do intervalo de 0 a 360 para -180 a 180
           
        }


        // proporcional
        msg_propulsorR.data = VEL_NOR + erro * VEL_P;
        msg_propulsorL.data = VEL_NOR - erro * VEL_P;

        printf ("GANHO: %.2f\n", erro*VEL_P);

        /*
        // manter longe das bordas
        if (distL < DIST_MIN){ // distancia esquerda
            // proporcional
            msg_propulsorL.data += distL * DIST_P;
            msg_propulsorR.data -= distR * DIST_P;
        }

        else if (distR < DIST_MIN){
            // proporcional
            msg_propulsorL.data -= (DIST_MIN - distL) * DIST_P;
            msg_propulsorR.data += (DIST_MIN - distR) * DIST_P;
        }        

         if (distF < DIST_MIN){ // distancia frente
            // proporcional
            msg_propulsorL.data -= distF * DIST_P;
            msg_propulsorR.data -= distB * DIST_P;
        }

        else if (distB < DIST_MIN){
            // proporcional
            msg_propulsorL.data += (DIST_MIN - distR) * DIST_P;
            msg_propulsorR.data += (DIST_MIN - distR) * DIST_P;
        }    
        */

        // limita as velocidades
        // motor ESQUERDO
        if (msg_propulsorL.data > VEL_MAX)
            msg_propulsorL.data = VEL_MAX;
        else if (msg_propulsorL.data < VEL_MIN)
            msg_propulsorL.data = VEL_MIN;
        // motor DIREITO
        if (msg_propulsorR.data > VEL_MAX)
            msg_propulsorR.data = VEL_MAX;
        else if (msg_propulsorR.data < VEL_MIN)
            msg_propulsorR.data = VEL_MIN;
       

        // muda a referencia de 0 a 510 para -255 a 255 (isso porque o define nao aceitou valor negativo)
        msg_propulsorR.data -= 255;
        msg_propulsorL.data -= 255;

		 // Pub lish the message .
		 pubR.publish(msg_propulsorR);
         pubL.publish(msg_propulsorL);   

		 // Wait un t i l i t ' s time for another i t e ra t ion .
		 rate.sleep();
		 ros::spinOnce();
 	}



}
