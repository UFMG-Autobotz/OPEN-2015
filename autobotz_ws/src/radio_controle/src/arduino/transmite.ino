
 /****************   TRANSMISSOR   **************************

 Instalação: http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

 Passo a Passo "ROS + Rádio":
 Verifique se o programa que está no catkin_ws/src/                beginner_tutorials/src é o que interpreta os dados do rádio.
 roscore <---NAO ESQUECER DE RODAR O MASTER
 rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0
 rosrun beginner_tutorials nodo             Para rodar o nó

 --- PINAGEM Comunicador---
 * (8) MISO -> 12
 * (5) MOSI -> 11
 * (6) SCK  -> 13
 * (4) CE   -> 8
 * (3) CSN  -> 7
 * (1) VCC  -> 3.3v
 * (2) GND  -> GND

 ----- INSTRUÇÕES DE MANUSEIO -----

* 'w' para acelerar os dois motores para FRENTE;
* 's' para desacelerar os dois motores; 
* 'e' para acelerar apenas o motor da direita;
* 'q' para acelerar apenas o da esqueda; 
* 'c' para desacelerar o da direita; 
* 'z' para desacelerar o da esquerda;
* 'd' incrementa o da direita e decrementa o da esquerda; 
* 'a' incrementa o da esquerda e decrementa o da direita. 
* 'r' para os dois motores (velocidade zero).

 **********************************************************/

#define Envio 1  // 1 - Para enviar mesma string somente uma vez
                 // (tenta varias até ter sucesso)
                 // 0 - Para enviar a mesma string indefinidamente
                 // (quando uma nova string for inserida, passa a envia-la)
#define CANAL 95 //Canal de comunicação
#define TamPayload 11 //tamanho da string em bytes
//#define TimeOut 30 //timeout da tentativa em ms

#include <SPI.h>
#include "Mirf.h"
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;
std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

char mensagem[TamPayload];
char ATB1[TamPayload] = "BA1000000B";
int tentativas = 0;
unsigned long time = 0;

void messageCb(const std_msgs::String& msg)
{  
  strcpy(ATB1, msg.data);
  EnviaString(ATB1, "ATBR1");
}

ros::Subscriber<std_msgs::String> sub("/eletronica/radio_controle/PWMs", messageCb);
unsigned int tempo;

void setup(){
  //Serial.begin(57600);
  //Serial.flush();
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ATBSV");
  Mirf.payload = TamPayload*sizeof(byte);
  Mirf.config();
  Mirf.channel = CANAL;
  nh.initNode();
  nh.subscribe(sub);
  //nh.advertise(chatter);
  //Serial.println("Sending..."); 
  tempo = millis();
}

void escreveData(char *string, byte *data){
  int i;
  int tam = TamPayload;
  if(strlen(string) < tam) tam = strlen(string);
  for(i = 0; i < tam; i++){
    data[i] = string[i];
  }
  while(i < TamPayload){//preenche o resto da string com espaço vazio
    data[i] = '\0';
    i++;
  }
}

int EnviaString(char *string, char Robo[5]){
  byte data[Mirf.payload];
  //Serial.println(string);
  escreveData(string, data); //escreve string em data
  Mirf.setTADDR((byte *)Robo);//configura receptor e envia
  Mirf.send((byte *)data);
  while(Mirf.isSending()){//enquanto tiver enviando, espera
  }
  delay(1);
  digitalWrite(A4, LOW);
  return 0; 
}

void loop()
{
  if((millis()-tempo) >= 1000){
      tempo = millis();
  }
  nh.spinOnce();
  delayMicroseconds(200);
} 
