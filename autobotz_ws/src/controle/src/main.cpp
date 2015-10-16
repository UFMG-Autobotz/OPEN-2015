/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
(31) 9405-5012
gustdomar@gmail.com

Código principal do pacote de controle
*/



// -------------------- BIBLIOTECAS --------------------

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Point.h>
#include <stdio.h>
#include <stdlib.h>


// ---------------- ARQUIVOS INCLUSOS ------------------

#include "controle/squareCenters.h" //custom message

// -------------------- CONSTANTES --------------------

#define TELA_LIN 480
#define TELA_COL 640
#define VEL_MAX 255
#define VEL_MIN -255
#define VEL_NOR 80 // intervalo de -255 a 255
#define VEL_RET 100
//#define VEL_P_ANG 1.2
//#define VEL_D_ANG 1.6
#define VEL_P_DIST 1.4
#define DIST_P 5.0
#define DIST_MIN 12.0


#define MODO 2
//#define ANG -20.3 // intervalo considerado de -180 a 180

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
    float erro_ang;
    float ang_anterior;

    if(!argv[1] || !argv[2] || !argv[3]){
        ROS_INFO("\n\nUse: rosrun nome_do_pacote control [Angulo desejado] [KP = constante proporcional do angulo] [KD = constante derivativa do angulo]\n\n");
        return -1;
    }


    // parametros recebidos na chamada do nó
    float ANG = atof(argv[0]);
    float VEL_P_ANG = atof(argv[1]);
    float VEL_D_ANG = atof(argv[2]);
    

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
            erro_ang = TELA_COL/2 - centerSquare.x;

        }

        else if(MODO == 2){ // segue um angulo desejado

            // calcula quao desalinhado o bloco esta do robo
            erro_ang = ANG - yaw; // calcula diferenca entre angulo desejado e real
           
        }


        // proporcional para distancia
        //msg_propulsorR.data = VEL_NOR;
        //msg_propulsorL.data = VEL_NOR;
        msg_propulsorR.data = 0;
        msg_propulsorL.data = 0;

        // proporcional para angulo
        msg_propulsorR.data += erro_ang * VEL_P_ANG;
        msg_propulsorL.data -= erro_ang * VEL_P_ANG;

        // derivativo para angulo
        msg_propulsorR.data += (yaw - ang_anterior) * VEL_D_ANG;
        msg_propulsorL.data -= (yaw - ang_anterior) * VEL_D_ANG;

        printf ("GANHO: %.2f\n", erro_ang *VEL_P_ANG);

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


/*
        // se esta chegando perto de um obstaculo, freia
        if (distF < DIST_MIN){

        	msg_propulsorR.data = VEL_NOR * (distF/DIST_MIN);

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
       


		 // Pub lish the message .
		 pubR.publish(msg_propulsorR);
         pubL.publish(msg_propulsorL);   


        // pega o valor anterior do angulo para uso do controle derivativo do angulo
        ang_anterior =  yaw;
         

		 // Wait until it's time for another iteration .
		 rate.sleep();
		 ros::spinOnce();
 	}



}
