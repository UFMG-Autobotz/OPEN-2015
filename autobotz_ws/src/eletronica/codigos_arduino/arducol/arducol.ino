/******************** ARDUCOL ************ ARDUCOL ******************
Motores do caracol:
  - IA: PWM 
     -Motor A: Pino 10
     -Motor B: Pino 9
     -Garra: Pino 6
  - IB: Direcao
     -Motor A: Pino 11
     -Motor B: Pino 8
     -Garra: Pino 7
  - VCC: 7V de uma bateria (No alimentar com o arduino)
  - GND: 0V, Lembre-se de conectar o TERRA da Bateria ao TERRA do Arduino

Comandos para rodar no ROS:
  ~$ rosrun rosserial_python serial_node.py /dev/ttyACM0
  ~$ rostopic echo /eletronica/garra/temBloco
  ~$ rostopic echo /controle/braco/motor/R
  ~$ rostopic echo /controle/braco/motor/L
  ~$ rostopic echo /controle/garra/motor

*******************************************************************/

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>

//variaveis de sesoriamento da GARRA
#define emissor 7  //pino do arduino onde se conecta o emissor infravermelho
#define receptor A0
typedef int contador;
contador loops_codigo  = 0, presenca_bloquinho = 0;

//variaveis da ponte H
int motor_garra[] = {6, 7};
int motor_left[] = {9, 8};
int motor_right[] = {10, 11};
int velocidadeR, velocidadeL, velocidadeG;
int direcaoR, direcaoL, direcaoG;
int auxiliarR, auxiliarL,auxiliarG;
#define TRAS 0
#define FRENTE 1
bool estado_garra;
ros::NodeHandle nh;

//Envia PWM e direcao ao motor da DIREITA
void messageR( const std_msgs::Int32& msg){
  auxiliarR = msg.data;
  if (auxiliarR < 0){
    velocidadeR = (-1) * auxiliarR;
    direcaoR = TRAS;
  }
  else{
    velocidadeR = auxiliarR;
    direcaoR = FRENTE;
  }
  if(direcaoR == FRENTE)
     velocidadeR = 255 - velocidadeR; 
}

//Envia PWM e direcao ao motor da ESQUERDA
void messageL( const std_msgs::Int32& msg){
 auxiliarL = msg.data;
  if (auxiliarL < 0){
    velocidadeL = (-1) * auxiliarL;
    direcaoL = TRAS;
  }
  else{
    velocidadeL = auxiliarL;
    direcaoL = FRENTE;
  }
    if(direcaoL == FRENTE)
     velocidadeL = 255 - velocidadeL;
}

//Envia PWM e direcao ao motor da GARRA
void messageGarra( const std_msgs::Int32& msg){
 auxiliarG = msg.data;
  if (auxiliarG < 0){
    velocidadeG = (-1) * auxiliarG;
    direcaoG = TRAS;
  }
  else{
    velocidadeG = auxiliarG;
    direcaoG = FRENTE;
  }
    if(direcaoG == FRENTE)
     velocidadeG = 255 - velocidadeG;
}

std_msgs::Bool bool_msg;
ros::Publisher chatter("/eletronica/garra/temBloco", &bool_msg );
ros::Subscriber<std_msgs::Int32> subR("/controle/braco/motor/R", &messageR );
ros::Subscriber<std_msgs::Int32> subL("/controle/braco/motor/L", &messageL );
ros::Subscriber<std_msgs::Int32> subGarra("/controle/garra/motor", &messageGarra );

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(subR);
  nh.subscribe(subL);
  nh.advertise(chatter);
  
  //Pinos de entrada e saída da PONTE-H do CARACOL
  int i;
  for(i = 0; i < 2; i++){
    pinMode(motor_left[i], OUTPUT);
    analogWrite(motor_left[i], 0);
    pinMode(motor_right[i], OUTPUT);
    analogWrite(motor_right[i], 0);
  }

  //Pinos de entrada e saída dos sensores da GARRA
  pinMode(emissor, OUTPUT);
  pinMode(receptor, INPUT);
}

void loop()
{
  //Sensoriamento da GARRA
  loops_codigo++;
  tone (emissor, 1000);
   int aux = analogRead(receptor);
  if(aux > 800) presenca_bloquinho++;

  if(presenca_bloquinho){
    estado_garra = true;
  }else{
    estado_garra = false;
  }

  if (loops_codigo >= 15){
    loops_codigo = 0;
    presenca_bloquinho = 0;
  }
  bool_msg.data = estado_garra; //Publica 0 para ausencia de bloco e 1 para presença

  //Envia os comandos para os motores do CARACOL
  digitalWrite(motor_left[1], direcaoL);  
  digitalWrite(motor_right[1], direcaoR);
  analogWrite(motor_left[0],velocidadeL);
  analogWrite(motor_right[0],velocidadeR);
  
  //Envia os comandos para o motor da GARRA
  digitalWrite(motor_garra[1], direcaoG);
  analogWrite(motor_garra[0],velocidadeG);
  
  chatter.publish( &bool_msg );
  
  nh.spinOnce();
  delay(200);
}