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

#include "controle/velocidade.h"
#include "./Funcoes/auxiliares.hpp"

// -------------------- CONSTANTES --------------------

//#define TELA_LIN 480
//#define TELA_COL 640

#define VEL_LIN_MAX 180
#define VEL_LIN_MIN -180
#define VEL_ANG_MAX 150
#define VEL_ANG_MIN -150
#define VEL_MAX 255
#define VEL_MIN -255
#define VEL_NOR 140 // intervalo de -255 a 255
#define VEL_RET 100
#define VEL_P_DIST 1.4
#define VEL_ANG_BRACO_KP 1.5
#define VEL_ANG_BRACO 80
#define VEL_LIN_BRACO 230
#define VEL_GARRA 160.0

#define DIST_P 5.8
#define DIST_MIN 5.0
#define DIST_FREIO 8.0

#define ERRO_ANG_MORTO 1
#define ERRO_ANG_OK 15

#define TEMPO_ALCANCA_BLOCO 200 // ms
#define TEMPO_AGARRA_BLOCO 3000 // ms
#define TEMPO_ESTENDE_BLOCO 3000 // ms
#define TEMPO_GUARDA_BRACO 5000 // ms
#define TEMPO_GUARDA_BLOCO 5000 // ms

#define ANG_BRACO_GUARDADO 110 // em graus



// ----------------- VARIÁVEIS GLOBAIS ------------------

int id_controlador;
bool start;
float distL, distR, distF, distB;
float yaw, destino, angulo_saida;
float bloco_objetivo_x;
float tela_x, tela_y;
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


 void yawNormalizado(const std_msgs::Float32& msg){

    yaw = msg.data;
    //printf ("YAW: %.2f\n", yaw);
 }


void velocidadeBarco(controle::velocidade msg){

    velBarco.linear = msg.linear;
    velBarco.angular = msg.angular;

}

void idControlador(std_msgs::Int32 msg){

    id_controlador = msg.data;

}

void destinoMsgRecieved(std_msgs::Float32 msg){

    destino = msg.data;

}

void blocoObjetivoXMsgRecieved(std_msgs::Float32 msg){

    bloco_objetivo_x = msg.data;

}

void anguloSaidaMsgRecieved(std_msgs::Float32 msg){

    angulo_saida = msg.data;

}

 void telaXMsgRecieved (std_msgs::Float32 msg){


    tela_x = msg.data;

 }

 void telaYMsgRecieved (std_msgs::Float32 msg){

    tela_y = msg.data;

 }

// ------------------ MAIN FUNCTION-------------------

int main(int argc, char **argv){



    // ------------ VARIAVEIS ------------
    
    start = false;
  
    float erro_ang, vel_ang_braco, vel_lin_braco, vel_garra;
    float destino_anterior;
    float linear_kp, linear_kd, angular_kp, angular_kd;


    geometry_msgs::Point objetivo;
    std::string const_dir("/home/constantes.txt");

/*
    if(argc < 3){
        ROS_INFO("\n\nUse: rosrun controle control_v2 [MODO] [ANG_BRACO]\n\n");
        return -1;
    }
*/
    // parametros recebidos na chamada do nó
//    float MODO = atof(argv[1]);
 //   float ANG_BARCO = atof(argv[2]);
    float MODO = 1;

    // carrega as cosntantes KP e KD, linear e angular, atraves de um arquivo (com nome armazenado em const_dir)
    leConstantesArquivo(const_dir, &linear_kp, &linear_kd, &angular_kp, &angular_kd);

/*
    // exibe as variaveis lidas
    printf ("\n------------ COSNTANTES ---------------\n\n");
    printf ("LINEAR: kp = %.2f kd = %.2f\n", linear_kp, linear_kd);
    printf ("ANGULAR: kp = %.2f kd = %.2f\n", angular_kp, angular_kd);
    printf ("\n---------------------------------------\n\n");
    printf("id_controlador = %d\n", id_controlador);
    printf("ANG BARCO= %f\n", ANG_BARCO);
    printf("ANG BRACO= %f\n", ANG_BRACO);
    printf ("\n---------------------------------------\n\n");
*/

    // init ROS stuff
    ros::init(argc, argv, "controle");

    // Establish this program as a ROS node
    ros::NodeHandle nh;


    // --------------------------- SUBSCRIBERS ---------------------------

    ros::Subscriber subR = nh.subscribe("eletronica/ultrassom/R", 1000, ultrassomR);
    ros::Subscriber subL = nh.subscribe("eletronica/ultrassom/L", 1000, ultrassomL);
    ros::Subscriber subF = nh.subscribe("eletronica/ultrassom/F", 1000, ultrassomF);
    ros::Subscriber subB = nh.subscribe("eletronica/ultrassom/B", 1000, ultrassomB);



    ros::Subscriber subIMU = nh.subscribe("estrategia/yawNormalizado", 1000, yawNormalizado);

    ros::Subscriber subVel = nh.subscribe("estrategia/velocidade", 1000, velocidadeBarco);

    ros::Subscriber subControlador = nh.subscribe("estrategia/estado_atual", 1000, idControlador);

    ros::Subscriber subDestino = nh.subscribe("estrategia/transporte/destino", 1000, destinoMsgRecieved);

    ros::Subscriber subBlocoObjetivoX = nh.subscribe("estrategia/bloco/bloco_objetivo_x", 1000, blocoObjetivoXMsgRecieved);


    ros::Subscriber subAnguloSaida = nh.subscribe("estrategia/transporte/angulo_saida", 1000, anguloSaidaMsgRecieved);


    ros::Subscriber subTelaX = nh.subscribe("/visao/screenX", 1000, &telaXMsgRecieved);
    ros::Subscriber subTelaY = nh.subscribe("/visao/screenY", 1000, &telaYMsgRecieved);


    // --------------------------- PUBLISHERS ---------------------------

    ros::Publisher pubR = nh.advertise <std_msgs::Int32>("eletronica/propulsor/R", 1000);
    ros::Publisher pubL = nh.advertise <std_msgs::Int32>("eletronica/propulsor/L", 1000);
    ros::Publisher pubBaseStepper = nh.advertise <std_msgs::Int32>("/controle/base/stepper", 1000);
    ros::Publisher pubBracoMotor = nh.advertise <std_msgs::Int32>("/controle/braco/motor/R", 1000); 
    ros::Publisher pubGarraMotor = nh.advertise <std_msgs::Int32>("/controle/garra/motor", 1000);   




    ros::Rate rate(2); // Hz

    std_msgs::Int32 msg_propulsorR, msg_propulsorL, msg_baseStepper, msg_bracoMotor, msg_garraMotor;


    while (ros::ok()){

        // zera propulsores
        msg_propulsorR.data = 0;
        msg_propulsorL.data = 0;
        msg_baseStepper.data = 0;
        msg_bracoMotor.data = 0;
        msg_garraMotor.data = 0;


        vel_ang_braco = 0.0;
        
        

        switch(id_controlador){

            case 0:
                    break;

            case 10: // estado ESCOLHER blocos
            
                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;


            case 11: // estado AJUSTA base

                // se o sensor ainda não está lendo bloco dentro da garra, estende o braço com velocidade constante
                
                // o braco vai reto e faz ajuste angular ao mesmo tempo
      //          vel_ang_braco = (bloco_objetivo_x - tela_x/2) * VEL_ANG_BRACO_KP;
                vel_lin_braco = 0;
        
       //         msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;


                msg_garraMotor.data = VEL_GARRA;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;


            case 12: // estado ESTENDER BRACO

                
                // se o sensor ainda não está lendo bloco dentro da garra, estende o braço com velocidade constante
                
                // o braco vai reto e faz ajuste angular ao mesmo tempo
        //        vel_ang_braco = (bloco_objetivo_x - tela_x/2) * VEL_ANG_BRACO_KP;
                vel_lin_braco = VEL_LIN_BRACO;
        
        //        msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;


                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 13: // estado AGARRAR
                           


                // para de mexer o braco, ja que alcançou o bloco
                vel_ang_braco = 0;
                vel_lin_braco = 0;

                vel_garra = (-1) * VEL_GARRA;
 
                msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;
                msg_garraMotor.data = vel_garra;


                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 14: // estado RECOLHER
                                    
                         
                // o braco vai reto e faz ajuste angular ao mesmo tempo
                //vel_ang_braco = ANG_BRACO * VEL_ANG_BRACO_KP;
                vel_lin_braco = (-1) * VEL_MAX;
                vel_garra = 0.0;
        
     //           msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;
                msg_garraMotor.data = vel_garra;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 15: // estado GUARDAR
                                    
                          
                // o braco vai reto e faz ajuste angular ao mesmo tempo
       //         vel_ang_braco = VEL_ANG_BRACO;
                vel_lin_braco = 0;
        
       //         msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;

                
                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 


            case 16: // estado CONTAR

               
                // o braco vai reto e faz ajuste angular ao mesmo tempo
                vel_ang_braco = 0;
        
                msg_baseStepper.data = vel_ang_braco;
       //    
                
                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX;                 


                break;

//##########################################################################################################

            case 20:  // DESATRACAR
            

                    if (MODO == 1){ // por angulo
                            erro_ang = angulo_saida - yaw; 
                            //erro_ang = distanciaAngularIMU (angulo_saida, yaw);
                            destino = yaw;
                        }

                    else if (MODO = 2)
                            erro_ang = destino - tela_x/2; // calcula diferenca entre angulo desejado e real



                    if (abs(erro_ang) >= ERRO_ANG_MORTO){
                        // proporcional para angulo
                        msg_propulsorR.data -= erro_ang * angular_kp;
                        msg_propulsorL.data += erro_ang * angular_kp;

                                                // derivativo para angulo
                        msg_propulsorR.data += (destino - destino_anterior) * angular_kd;
                        msg_propulsorL.data -= (destino - destino_anterior) * angular_kd;



                    }


                    printf ("GANHO: %.2f\n", erro_ang *angular_kp);



                    break;

            case 21: // LOCALIZA OBJETIVO
            case 22: // NAVEGA
                    
                    if (MODO == 1){
                            //erro_ang = angulo_saida - yaw; 
                            erro_ang = distanciaAngularIMU (angulo_saida, yaw);
                    }
                    else if (MODO = 2)
                            erro_ang = destino - tela_x/2; // calcula diferenca entre angulo desejado e real



                    if (abs(erro_ang) <= ERRO_ANG_OK){
                        // acrescenta velocidade linear
                        //msg_propulsorR.data += VEL_NOR * abs(ERRO_ANG_OK - erro_ang);
                        //msg_propulsorL.data += VEL_NOR * abs(ERRO_ANG_OK - erro_ang);
                        msg_propulsorR.data += VEL_NOR * (1.0 - erro_ang / ERRO_ANG_OK);
                        msg_propulsorL.data += VEL_NOR * (1.0 - erro_ang / ERRO_ANG_OK);
                    }


                    if (abs(erro_ang) >= ERRO_ANG_MORTO){
                        // proporcional para angulo
                        msg_propulsorR.data -= erro_ang * angular_kp;
                        msg_propulsorL.data += erro_ang * angular_kp;

                        // derivativo para angulo
                        msg_propulsorR.data += (destino - destino_anterior) * angular_kd;
                        msg_propulsorL.data -= (destino - destino_anterior) * angular_kd;

                    }



                    printf ("GANHO: %.2f\n", erro_ang *angular_kp);

                    
               /*     // MANTEM LONGE DAS BORDAS
                    if (distL < DIST_MIN){ // distancia esquerda
                        // proporcional
                        msg_propulsorL.data += (DIST_MIN - distL) * DIST_P;
                        msg_propulsorR.data -= (DIST_MIN - distR) * DIST_P;
                    }
                
                    else if (distR < DIST_MIN){
                        // proporcional
                        msg_propulsorL.data -= (DIST_MIN - distL) * DIST_P;
                        msg_propulsorR.data += (DIST_MIN - distR) * DIST_P;
                    }        

                     if (distF < DIST_MIN){ // distancia frente
                        // proporcional
                        msg_propulsorL.data -= (DIST_MIN - distF) * DIST_P;
                        msg_propulsorR.data -= (DIST_MIN - distB) * DIST_P;
                    }*/

                /*    else if (distB < DIST_MIN){
                        // proporcional
                        msg_propulsorL.data += (DIST_MIN - distR) * DIST_P;
                        msg_propulsorR.data += (DIST_MIN - distR) * DIST_P;
                    }    
                    */


            
                    // se esta chegando perto de um obstaculo, freia
                    /* if (distF < DIST_MIN && distF > DIST_FREIO){

                        msg_propulsorR.data = VEL_NOR * (distF/DIST_MIN);
                        msg_propulsorL.data = VEL_NOR * (distF/DIST_MIN);

                    }
                    else if ((distF <= DIST_FREIO && distF >= 0) && erro_ang <= ERRO_ANG_MORTO){

                        
                        msg_atracado.data = true;

                        msg_propulsorR.data = 0.0;
                        msg_propulsorL.data = 0.0;


                    }
               */

                    break;

          
//#######################################################################################################

            // estado DEIXAR BLOCO


            case 30: // acha LUGAR DISPONIVEL 

                    // mantem o barco atracado
                    msg_propulsorR.data = VEL_MAX;
                    msg_propulsorL.data = VEL_MAX; 

                    break;

            case 31: // estado ESTENDER BRACO

                
                // se o sensor ainda não está lendo bloco dentro da garra, estende o braço com velocidade constante
                
                // o braco vai reto e faz ajuste angular ao mesmo tempo
     //           vel_ang_braco = ANG_BRACO * VEL_ANG_BRACO_KP;
                vel_lin_braco = VEL_LIN_BRACO;
        
                // atualiza o que sera publicado
                msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 32: // estado SOLTAR bloco
                                    
                // espera um tempo para a garra alcançar melhor o bloco
      //          sleep(TEMPO_ESTENDE_BLOCO);
                // para de mexer o braco, ja que alcançou o bloco
                vel_ang_braco = 0;
                vel_lin_braco = 0;
                vel_garra = VEL_GARRA;
 

                // atualiza o que sera publicado
                msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;
                msg_garraMotor.data = vel_garra;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 33: // estado RECOLHER
                                    
                // espera um tempo para a garra pegar melhor o bloco
        //        sleep(TEMPO_SOLTA_BLOCO);
                         
                // o braco vai reto e faz ajuste angular ao mesmo tempo
                //vel_ang_braco = ANG_BRACO * VEL_ANG_BRACO_KP;
                vel_lin_braco = (-1) * VEL_LIN_BRACO;
                vel_garra = 0.0;
        
                // atualiza o que sera publicado
                msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;
                msg_garraMotor.data = vel_garra;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;

            case 34: //estado GUARDAR
                                    
                          
                // o braco vai reto e faz ajuste angular ao mesmo tempo
                vel_ang_braco = VEL_ANG_BRACO;
                vel_lin_braco = 0;

                // atualiza o que sera publicado
                msg_baseStepper.data = vel_ang_braco;
                msg_bracoMotor.data = vel_lin_braco;

                // mantem o barco atracado
                msg_propulsorR.data = VEL_MAX;
                msg_propulsorL.data = VEL_MAX; 

                break;


            default:
                msg_garraMotor.data = 0;
                msg_baseStepper.data = 0;
                msg_bracoMotor.data = 0;          
                msg_propulsorR.data = 0;
                msg_propulsorL.data = 0;      
                break;


        }

        // --------------- LIMITA AS VELOCIDADES ----------------

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

        // Stepper da base
        if (msg_baseStepper.data > VEL_MAX)
            msg_baseStepper.data = VEL_MAX;
        else if (msg_baseStepper.data < VEL_MIN)
            msg_baseStepper.data = VEL_MIN;

        // Motor do braço
        if (msg_bracoMotor.data > VEL_MAX)
            msg_bracoMotor.data = VEL_MAX;
        else if (msg_bracoMotor.data < VEL_MIN)
            msg_bracoMotor.data = VEL_MIN;
        

        // Motor da garra
        if (msg_garraMotor.data > VEL_MAX)
            msg_garraMotor.data = VEL_MAX;
        else if (msg_garraMotor.data < VEL_MIN)
            msg_garraMotor.data = VEL_MIN;


        msg_propulsorL.data *= (-1);
        msg_propulsorR.data *= (-1);

        // ------------------------------------------------------

        // Publish the message .
        pubR.publish(msg_propulsorR);
        pubL.publish(msg_propulsorL);   
        pubBaseStepper.publish(msg_baseStepper);
        pubBracoMotor.publish(msg_bracoMotor);
        pubGarraMotor.publish(msg_garraMotor);



        // pega o valor anterior do angulo para uso do controle derivativo do angulo
        destino_anterior =  destino;
        if (MODO == 1){

            destino_anterior = yaw;
        }

         // Wait until it's time for another iteration .
         rate.sleep();
         ros::spinOnce();
    }



}
