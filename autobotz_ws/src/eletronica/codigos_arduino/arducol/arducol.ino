//Includes do ROS
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>

//Variáveis da GARRA
bool estado_sensor;

//Variaveis da ponte H do caracol
int motor_left[] = {8, 9}; //8: direcao, 9: PWM
int motor_right[] = {10, 11};//10: direcao, 11: PWM
int velocidadeR, velocidadeL;
int direcaoR, direcaoL;
int auxiliarR, auxiliarL;
#define TRAS 0
#define FRENTE 1

//CallBack da velocidade e direção do motor da direita
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
}

//CallBack da velocidade e direção do motor da esquerda
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
}

ros::NodeHandle n;
std_msgs::Bool bool_msg;
ros::Publisher chatter("/eletronica/garra/temBloco", &bool_msg);
ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int32> subR("/controle/braco/motor/R", &messageR );
ros::Subscriber<std_msgs::Int32> subL("/controle/braco/motor/L", &messageL );

void setup() {

  n.initNode();
  n.advertise(chatter);
  
  nh.initNode();
  nh.subscribe(subR);
  nh.subscribe(subL);
  
  //Definição dos pinos de saída da Ponte-H
  int i;
  for(i = 0; i < 2; i++){ 
    pinMode(motor_left[i], OUTPUT);
    analogWrite(motor_left[i], 0);
    pinMode(motor_right[i], OUTPUT);
    analogWrite(motor_right[i], 0);
  }
  
  //Definição dos pinos de entrada/saída da GARRA

 }


void loop() {
  
  if (velocidadeL>255)
    velocidadeL = 255;
   
  if (velocidadeR>255)
    velocidadeR = 255;

  digitalWrite(motor_left[0], direcaoL);  
  digitalWrite(motor_right[0], direcaoR);
  analogWrite(motor_left[1],velocidadeL);
  analogWrite(motor_right[1],velocidadeR);
  
  if(estado_sensor == 1)
    bool_msg.data = true;
  else 
    bool_msg.data = false;
    
  chatter.publish( &bool_msg );
  
  n.spinOnce();
  nh.spinOnce();
  delay(100);             
}
