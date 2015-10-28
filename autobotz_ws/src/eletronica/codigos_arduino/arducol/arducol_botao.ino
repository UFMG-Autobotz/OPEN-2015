/******************** ARDUCOL+ BOTAO *********************************
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
#define emissor 4  //pino do arduino onde se conecta o emissor infravermelho
#define receptor A0
typedef int contador;
contador loops_codigo  = 0, presenca_bloquinho = 0;

//Variaveis do botao
int BotaoPin = 2;

//variaveis da ponte H
int motor_garra[] = {10,11};
int motor_left[] = {3, 8, 9};
int motor_right[] = {5, 6, 7 };
int velocidadeR, velocidadeL, velocidadeG;
int direcaoR,  direcaoG;
int direcaoR1, direcaoR2;
int auxiliarR,auxiliarG;
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
  //velocidadeR = (velocidadeR/255.0)*1000.0;
     
}

////Envia PWM e direcao ao motor da GARRA
void messageGarra( const std_msgs::Int32& msg){
 auxiliarG = msg.data;
 
 if(auxiliarG == 0)
    velocidadeG = 0;
 else
    velocidadeG = 255;
    
  if (auxiliarG < 0)
    direcaoG = TRAS;
  else
    direcaoG = FRENTE;
}

std_msgs::Bool bool_msg;
std_msgs::Bool botao_msg;

ros::Publisher chatter_botao("/eletronica/start", &botao_msg);

//ros::Publisher chatter("/eletronica/garra/temBloco", &bool_msg );
ros::Subscriber<std_msgs::Int32> subR("/controle/braco/motor/R", &messageR );
//ros::Subscriber<std_msgs::Int32> subL("/controle/braco/motor/L", &messageL );
ros::Subscriber<std_msgs::Int32> subGarra("/controle/garra/motor", &messageGarra );

std_msgs::Int32 msg_teste;
ros::Publisher teste("/teste", &msg_teste );

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(subR);
  //nh.subscribe(subL);
  nh.subscribe(subGarra);
  nh.advertise(teste);
  nh.advertise(chatter_botao);

  pinMode(BotaoPin, INPUT); 
  
  //Pinos de entrada e saída da PONTE-H do CARACOL
  int i;
  for(i = 0; i < 3; i++){
    pinMode(motor_left[i], OUTPUT);
    analogWrite(motor_left[i], 0);
    pinMode(motor_right[i], OUTPUT);
    analogWrite(motor_right[i], 0);
  }
  pinMode(motor_garra[0], OUTPUT);
  analogWrite(motor_garra[0], 0);
  pinMode(motor_garra[1], OUTPUT);
  analogWrite(motor_garra[1], 0);

  //Pinos de entrada e saída dos sensores da GARRA
  pinMode(emissor, OUTPUT);
  pinMode(receptor, INPUT);
}

void loop()
{
//  //Sensoriamento da GARRA
//  loops_codigo++;
//  tone (emissor, 1000);
//   int aux = analogRead(receptor);
//  if(aux > 800) presenca_bloquinho++;
//
//  if(presenca_bloquinho){
//    estado_garra = true;
//  }else{
//    estado_garra = false;
//  }
//
//  if (loops_codigo >= 15){
//    loops_codigo = 0;
//    presenca_bloquinho = 0;
//  }
  //bool_msg.data = estado_garra; //Publica 0 para ausencia de bloco e 1 para presenç
  

if(direcaoR == TRAS){
  digitalWrite(13, HIGH);
  analogWrite(motor_left[0],velocidadeR);
  digitalWrite(motor_left[2], 0);  
  digitalWrite(motor_left[1], 1);
  
  analogWrite(motor_right[0],velocidadeR);
  digitalWrite(motor_right[2], 0);   
  digitalWrite(motor_right[1], 1);
}
  //delay(2000);
  
if(direcaoR == FRENTE){
  digitalWrite(13, LOW);
  analogWrite(motor_left[0],velocidadeR);
  digitalWrite(motor_left[2], 1);  
  digitalWrite(motor_left[1], 0);
  
  analogWrite(motor_right[0],velocidadeR);
  digitalWrite(motor_right[2], 1);   
  digitalWrite(motor_right[1], 0);
} 
  //delay(2000);
  
//  //Envia os comandos para o motor da GARRA
  if(direcaoG == TRAS){
  digitalWrite(motor_garra[1], 0);
  analogWrite(motor_garra[0], velocidadeG);
  //digitalWrite(motor_garra[0],1);
  }
  
  if(direcaoG == FRENTE){
  //digitalWrite(motor_garra[1], 1);
  analogWrite(motor_garra[1], velocidadeG);
  digitalWrite(motor_garra[0],0);
  }

  botao_msg.data= digitalRead(BotaoPin);
  chatter_botao.publish( &botao_msg );
  
//  msg_teste.data = velocidadeR;
//  teste.publish( &msg_teste );
 
  nh.spinOnce();
  delay(200);
}