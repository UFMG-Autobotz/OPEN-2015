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
#include <time.h>


// ---------------- ARQUIVOS INCLUSOS ------------------

#include "controle/squareCenters.h" //custom message
#include "controle/velocidade.h"
#include "./Funcoes/auxiliares.hpp"

// -------------------- CONSTANTES --------------------

#define TELA_LIN 480
#define TELA_COL 640

#define VEL_LIN_MAX 180
#define VEL_LIN_MIN -180
#define VEL_ANG_MAX 150
#define VEL_ANG_MIN -150
#define VEL_MAX 50
#define VEL_MIN -50
#define VEL_NOR 12 // intervalo de -255 a 255
#define VEL_RET 100
#define VEL_P_DIST 1.4

#define DIST_P 5.0
#define DIST_MIN 5.0
#define DIST_FREIO 8.0

#define ERRO_ANG_MORTO 1
#define ERRO_ANG_OK 45




// ----------------- VARIÁVEIS GLOBAIS ------------------


bool start;
int id_controlador;
float distL, distR, distF, distB;
float yaw;
geometry_msgs::Point centerSquare;
controle::velocidade velBarco, velBarco_anterior;



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


void velocidadeBarco(controle::velocidade msg){

    velBarco.linear = msg.linear;
    velBarco.angular = msg.angular;

}

void idControlador(std_msgs::Int32 msg){

    id_controlador = msg.data;

}


// tem que refazer isso para o novo tipo de mensagem do Blanc
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
  
    float erro_ang;
    float ang_anterior;
    float linear_kp, linear_kd, angular_kp, angular_kd;

    // para calcular tempo
    //clock_t time1 = 0.0;

    geometry_msgs::Point objetivo;
    std::string const_dir("/home/gustavo/open-2015/autobotz_ws/src/controle/src/constantes.txt");


    if(argc < 2){
        ROS_INFO("\n\nUse: rosrun controle control [Angulo desejado]\n\n");
        return -1;
    }

    // parametros recebidos na chamada do nó
    float ANG = atof(argv[1]);


    // carrega as cosntantes KP e KD, linear e angular, atraves de um arquivo (com nome armazenado em const_dir)
    leConstantesArquivo(const_dir, &linear_kp, &linear_kd, &angular_kp, &angular_kd);


    // exibe as variaveis lidas
    printf ("\n------------ COSNTANTES ---------------\n\n");
    printf ("LINEAR: kp = %.2f kd = %.2f\n", linear_kp, linear_kd);
    printf ("ANGULAR: kp = %.2f kd = %.2f\n", angular_kp, angular_kd);
    printf ("\n---------------------------------------\n\n");
    printf("id_controlador = %d\n", id_controlador);
    printf("ANG = %f\n", ANG);
    printf ("\n---------------------------------------\n\n");


	// init ROS stuff
	ros::init(argc, argv, "controle");

	// Establish this program as a ROS node
    ros::NodeHandle nh;


    // --------------------------- SUBSCRIBERS ---------------------------

    ros::Subscriber subR = nh.subscribe("eletronica/ultrassom/R", 1000, ultrassomR);
    ros::Subscriber subL = nh.subscribe("eletronica/ultrassom/L", 1000, ultrassomL);
    ros::Subscriber subF = nh.subscribe("eletronica/ultrassom/F", 1000, ultrassomF);
    ros::Subscriber subB = nh.subscribe("eletronica/ultrassom/B", 1000, ultrassomB);

    // no codigo oficial vai ler de um topico da estrategia que publica o bloco a pegar
    ros::Subscriber sub = nh.subscribe("visao/squares/centers", 1000, cubosAmarelos);

    ros::Subscriber subIMU = nh.subscribe("eletronica/imu/yaw", 1000, imu);

    ros::Subscriber subVel = nh.subscribe("estrategia/velocidade", 1000, velocidadeBarco);

    ros::Subscriber subControlador = nh.subscribe("estrategia/id_controlador", 1000, idControlador);


    // --------------------------- PUBLISHERS ---------------------------

    ros::Publisher pubR = nh.advertise <std_msgs::Int32>("eletronica/propulsor/R", 1000);
    ros::Publisher pubL = nh.advertise <std_msgs::Int32>("eletronica/propulsor/L", 1000);
    ros::Publisher pubAtracado = nh.advertise <std_msgs::Bool>("controle/barco/atracado ", 1000);



    ros::Rate rate(2); // Hz

    std_msgs::Int32 msg_propulsorR, msg_propulsorL;
    std_msgs::Bool msg_atracado;


    while (ros::ok()){

        // zera propulsores
        msg_propulsorR.data = 0;
        msg_propulsorL.data = 0;
		
        msg_atracado.data = false;

        switch(id_controlador){

            case 0:
                    break;
            case 1:
                    // calcula quao desalinhado o bloco esta do robo
                    erro_ang = TELA_COL/2 - centerSquare.x;

                    break;
            case 2:
                    // calcula quao desalinhado o bloco esta do robo
                    erro_ang = ANG - yaw; // calcula diferenca entre angulo desejado e real



                    if (abs(erro_ang) <= ERRO_ANG_OK){
                        // acrescenta velocidade linear
                        //msg_propulsorR.data += VEL_NOR * abs(ERRO_ANG_OK - erro_ang) * linear_kp ;
                        //msg_propulsorL.data += VEL_NOR * abs(ERRO_ANG_OK - erro_ang) * linear_kp ;

                    }


                    if (abs(erro_ang) >= ERRO_ANG_MORTO){
                        // proporcional para angulo
                        msg_propulsorR.data -= erro_ang * angular_kp;
                        msg_propulsorL.data += erro_ang * angular_kp;

                        // derivativo para angulo
                        msg_propulsorR.data += (yaw - ang_anterior) * angular_kd;
                        msg_propulsorL.data -= (yaw - ang_anterior) * angular_kd;

                    }

                    printf ("GANHO: %.2f\n", erro_ang *angular_kp);

                    /*
                    // MANTEM LONGE DAS BORDAS
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


            
                    // se esta chegando perto de um obstaculo, freia
                    if (distF < DIST_MIN && distF > DIST_FREIO){

                        msg_propulsorR.data = VEL_NOR * (distF/DIST_MIN);
                        msg_propulsorL.data = VEL_NOR * (distF/DIST_MIN);

                    }
                    else if ((distF <= DIST_FREIO && distF >= 0) && erro_ang <= ERRO_ANG_MORTO){

                        
                        msg_atracado.data = true;

                        msg_propulsorR.data = 0.0;
                        msg_propulsorL.data = 0.0;


                    }
               

                    break;

            case 3:

                    // linear
                    if (velBarco.linear.data > VEL_LIN_MAX)
                        velBarco.linear.data = VEL_LIN_MAX;
                    else if (velBarco.linear.data < VEL_LIN_MIN)
                        velBarco.linear.data = VEL_LIN_MIN;
                    // angular
                    if (velBarco.angular.data > VEL_ANG_MAX)
                        velBarco.angular.data = VEL_ANG_MAX;
                    else if (velBarco.angular.data < VEL_ANG_MIN)
                        velBarco.angular.data = VEL_ANG_MIN;


                    msg_propulsorR.data += (velBarco.linear.data + velBarco.angular.data) * angular_kp;
                    msg_propulsorL.data += (velBarco.linear.data - velBarco.angular.data) * angular_kp;

                    // derivativo 
                    msg_propulsorR.data += (velBarco.linear.data - velBarco_anterior.linear.data) * angular_kd;
                    msg_propulsorL.data += (velBarco.linear.data - velBarco_anterior.linear.data) * angular_kd;
                    msg_propulsorR.data -= (velBarco.angular.data - velBarco_anterior.angular.data) * angular_kd;
                    msg_propulsorL.data -= (velBarco.angular.data - velBarco_anterior.angular.data) * angular_kd;

                    printf ("GANHO: %.2f\n", erro_ang *angular_kp);



                    break;



            default:

                    break;



        }

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
       
        // provisorio
        msg_propulsorL.data *= -1;

		// Pub lish the message .
		pubR.publish(msg_propulsorR);
        pubL.publish(msg_propulsorL);   
        pubAtracado.publish(msg_atracado);


        // pega o valor anterior do angulo para uso do controle derivativo do angulo
        ang_anterior =  yaw;
        velBarco_anterior = velBarco;

        /* Como calcular tempo em C++
        printf ("\n\nDelta t: %f\n\n", float(std::clock() - time1)/CLOCKS_PER_SEC);
        time1 = std::clock();
        sleep(100); // em milesegundos
        */
		 // Wait until it's time for another iteration .
		 rate.sleep();
		 ros::spinOnce();
 	}



}
