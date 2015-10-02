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




// ---------------- ARQUIVOS INCLUSOS ------------------



// -------------------- CONSTANTES --------------------

#define TELA_LIN 480
#define TELA_COL 640
#define VEL_MAX 510
#define VEL_MIN 0
#define VEL_NOR 355
#define VEL_P 1.5
#define DIST_P 5.0
#define DIST_MIN 12.0

// ----------------- VARIÁVEIS GLOBAIS ------------------

bool start;
float distL, distR, distF, distB;
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

void cubosAmarelos (const geometry_msgs::Point& msg){

    centerSquare = msg;

 }

// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){


	// ------------ VARIAVEIS ------------
	start = false;
    geometry_msgs::Point objetivo;
    int erro;



	// init ROS stuff
	ros::init(argc, argv, "controle");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    ros::Subscriber subR = nh.subscribe("eletronica/ultrassom/R", 1000, &ultrassomR);
    ros::Subscriber subL = nh.subscribe("eletronica/ultrassom/L", 1000, &ultrassomL);
    ros::Subscriber subF = nh.subscribe("eletronica/ultrassom/F", 1000, &ultrassomF);
    ros::Subscriber subB = nh.subscribe("eletronica/ultrassom/B", 1000, &ultrassomB);

    ros::Subscriber sub = nh.subscribe("visao/squares/centers", 1000, &cubosAmarelos);


    ros::Publisher pubR = nh.advertise <std_msgs::Int32>("eletronica/propulsor/R", 1000);
    ros::Publisher pubL = nh.advertise <std_msgs::Int32>("eletronica/propulsor/L", 1000);
    ros::Rate rate(2); // Hz

    std_msgs::Int32 msg_propulsorR, msg_propulsorL;

    while (ros::ok()){

		 
        // calcula quao desalinhado o bloco esta do robo
        erro = TELA_COL/2 - centerSquare.x;
        // proporcional
        msg_propulsorR.data = VEL_NOR + erro * VEL_P;
        msg_propulsorL.data = VEL_NOR - erro * VEL_P;


        // manter longe das bordas
        if (distL < DIST_MIN){ // distancia esquerda
            // proporcional
            msg_propulsorL.data += distL * DIST_P;
            msg_propulsorR.data -= distL * DIST_P;
        }

        else if (distR < DIST_MIN){
            // proporcional
            msg_propulsorL.data -= (DIST_MIN - distR) * DIST_P;
            msg_propulsorR.data += (DIST_MIN - distR) * DIST_P;
        }

        if (msg_propulsorL.data > VEL_MAX)
            msg_propulsorL.data = VEL_MAX;
        else if (msg_propulsorL.data < VEL_MIN)
            msg_propulsorL.data = VEL_MIN;
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
