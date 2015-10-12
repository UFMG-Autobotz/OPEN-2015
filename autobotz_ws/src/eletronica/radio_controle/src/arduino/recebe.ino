
 /*****************   RECEPTOR   **************************

 ESTE CÓDIGO É USADO JUNTO AO CÓDIGO transmite4.0.txt!

 Instalação: http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

 Passo a Passo "ROS + Rádio":
 Verifique se o programa que está no catkin_ws/src/                beginner_tutorials/src é o que interpreta os dados do rádio.
 roscore <---NAO ESQUECER DE RODAR O MASTER
 rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0
 rosrun beginner_tutorials nodo             Para rodar o nó

 ----- PINAGEM Comunicador-----
 * (8) MISO -> 12
 * (5) MOSI -> 11
 * (6) SCK  -> 13
 * (4) CE   -> 8
 * (3) CSN  -> 7
 * (1) VCC  -> 3.3v
 * (2) GND  -> GND

 ----- PINAGEM ponte H -----
 * Enable 1 e 2 (1 e 9) e Vss -> 5v
 * Vs                         -> Polo positivo da bateria
 * GND (4, 5, 13 e 12)        -> GND
 * Output 1 e 2 (3 e 6)       -> Motor da esquerda
 * Output 3 e 4 (14 e 11)     -> Motor da direita
 * Input 1 e 2 (2 e 7)        -> motor_left[]
 * Input 3 e 4 (10 e 15)      -> motor_right[]

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

#define CANAL 95 //canal de comunicação
#define TamPayload 11 //tamanho da string em bytes
#define NomeRobo "ATBR1"
#define NumRobo '1'
#define TempMedicao 30 //intervalo entre as medições da bateria (segundos)

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
//variaveis do voltimetro
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 10000.0;    // !! resistance of R1 !!
float R2 = 10000.0;     // !! resistance of R2 !!
int Analog1 = 0;
float Lim_Tensao = 7.4;
float time = -30000;
//variaveis dos leds do debugger
int LED_RCB = 2;
//int LED_BAT = 3;
//int LED_ERR = 4;
//variaveis da ponte H
int motor_left[] = {6, 5};
int motor_right[] = {10, 9};
//Variavel de envio
char relatorio[TamPayload];


void setup(){
  pinMode(analogInput, INPUT);
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)NomeRobo);
  Mirf.payload = TamPayload*sizeof(byte);
  Mirf.config();
  Mirf.channel = CANAL;
  // Setup motors
  int i;
  for(i = 0; i < 2; i++){
    pinMode(motor_left[i], OUTPUT);
    analogWrite(motor_left[i], 0);
    pinMode(motor_right[i], OUTPUT);
    analogWrite(motor_right[i], 0);
  }
  Serial.println("Listening...");
  pinMode(LED_RCB, OUTPUT);
//  pinMode(LED_ERR, OUTPUT);
//  pinMode(LED_BAT, OUTPUT);
}

//float nivelBat(){
//  //Verifica Tensão
//  Analog1 = analogRead(analogInput);
//  vout = (Analog1 * 5.0) / 1024.0;
//  //vin = vout / (R2/(R1+R2));
//  vin = vout*(R1+R2)/R2;
//  //se for menor que o limite, acende o LED
//  if(vin < Lim_Tensao)digitalWrite(LED_BAT, HIGH);
//  else digitalWrite(LED_BAT, LOW);
//  return vin;
//}

int RecebeString(byte * data){
  //recebe dados
  if(!Mirf.isSending() && Mirf.dataReady()){
  //salva em data
  Mirf.getData(data);
    return 1;
  }
  return 0;
}

int verifica(char string[TamPayload]){
  //posição dos colchetes
  if(string[0] != 'B' || string[9] != 'B') return 0;
  //codigo do robo
  if(string[1] != 'A' || string[2] != NumRobo) return 0;
  //numeros do robo
  for(int i = 3; i < 9; i++){
    if(string[i] > '9' || string[i] < '0') return 0;
  }
  //numeros da centena
  if(string[3] > '5' || string [3] < '0') return 0;
  if(string[6] > '5' || string [6] < '0') return 0;

  return 1;
}

void infotoint(int *dir_esq, int *pwm_esq, int *dir_dir, int *pwm_dir, char string[TamPayload]){
  char temp[4];
  int i;
//converte primeiros numeros pra int (para motor esquerdo)
    for(i = 0; i < 3; i++){
        temp[i] = string[i+3];
    }
    temp[i] = '\0';

    *pwm_esq = atoi(temp);
    //analisa e decodifica
    if(*pwm_esq <= 255) *dir_esq = 1; //se for menor que 255, direcao recebe frente (1)
    else {//se for maior que 255, direcao recebe 0 (para traz) e o valor pwm é normalizado para 255
      *dir_esq = 0;
      *pwm_esq -= 255;
    }
//converte informaçoes do lado direito (motor da direita)
    for(i = 0; i < 3; i++){
        temp[i] = string[i+6];
    }
    temp[i] = '\0';

    *pwm_dir = atoi(temp);
    //analisa e decodifica
    if(*pwm_dir <= 255) *dir_dir = 1; //se for menor que 255, direcao recebe frente (1)
    else {//se for maior que 255, direcao recebe 0 (para traz) e o valor pwm é normalizado para 255
      *dir_dir = 0;
      *pwm_dir -= 255;
    }



}

void ConfMotor(int dir_esq, int pwm_esq, int dir_dir, int pwm_dir){
  // se dir_esq/dir_dir for 1 , motor vai pra frente, se for 0 vai pra traz
  
  //motor da esquerda
  analogWrite(motor_left[0], pwm_esq);
  if(dir_esq)
      digitalWrite(motor_left[1], HIGH);
  else
      digitalWrite(motor_left[1], LOW); 
      
  //motor da direita
  analogWrite(motor_right[0], pwm_dir);
  if(dir_dir)
      digitalWrite(motor_right[1], HIGH);
  else
      digitalWrite(motor_right[1], LOW);
}

void escreveData(char *string, byte *data){
  int i;
  int tam = TamPayload;
  if(strlen(string) < tam) tam = strlen(string);
  for(i = 0; i < tam; i++){
    data[i] = string[i];
  }
  //preenche o resto da string com espaço vazio
  while(i < TamPayload){
    data[i] = '\0';
    i++;
  }
}

int nRel = 0; 
int envios = 0;
int falhas = 0;


void loop(){
  envios++;
  int i;
  char temp[TamPayload];
  byte data[Mirf.payload];
//  float bat;
  //variaveis do motor
  int pwm_esq, pwm_dir, dir_esq, dir_dir;

  //Configura pra recebimento
  Mirf.setRADDR((byte *)NomeRobo);
  //fica em standby enquanto nao recebe informações
  while(!RecebeString(data)){digitalWrite(LED_RCB, LOW);}

  digitalWrite(LED_RCB, HIGH);
  delay(1);
  //escreve dados recebidos numa string
  for(i = 0; i < TamPayload; i++){
    temp[i] = data[i];
  }
  temp[i] = '\0';
  //Serial.println(temp);
  
   //Verifica integridade da string, se for válido, configura o motor
  if(verifica(temp)){

  //digitalWrite(LED_ERR, LOW);
  // traduz string pra inteiros
  //Serial.println(temp);
    infotoint(&dir_esq, &pwm_esq, &dir_dir, &pwm_dir, temp);

  //reconfigura motor
  
  ConfMotor(dir_esq, pwm_esq, dir_dir, pwm_dir);
  
  Serial.print("  Pwm_esq:  ");
  Serial.print(pwm_esq);  
  Serial.print("  Pwm_dir  ");
  Serial.print(pwm_dir);
  
  Serial.print("     Dir_esq:  ");
  Serial.print(dir_esq);  
  Serial.print("  Dir_dir  ");
  Serial.println(dir_dir);
   
  ConfMotor(dir_esq, pwm_esq, dir_dir, pwm_dir);
  
  }
  else {
    //digitalWrite(LED_ERR, HIGH);
    delay(1);
    falhas++;
  }

/*
  //Envia relatório de tempo em tempo
  if((millis() - time) > TempMedicao*1000){
    //configura para envio 
    Mirf.setTADDR((byte *)"ATBSR");
    
    //processa informações e escreve o relatório
      //nome do robo
      bat = nivelBat();
      //sprintf(relatorio,"A%c %05i %03i",NumRobo,nRel++,(int)(bat*100));
      sprintf(relatorio,"%c R%04i V%03i E%04i F%04i",NumRobo,nRel,(int)(bat*100),envios, falhas);
      //Serial.println(relatorio);
    //Envia informações pro receptor (algumas vezes)
    for (int i = 0; i < 1 ;i++){
      Mirf.send((byte*)relatorio);
      while(Mirf.isSending()){}
    }*/
    //zera variavel do contador
    time = millis();
    nRel++;
    falhas = 0;
    envios = 0;
    
  //}
  //recomeça loop e espera proxima ordem
}
