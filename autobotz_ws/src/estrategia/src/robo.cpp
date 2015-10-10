/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

Descrição do arquivo
*/



// -------------------- BIBLIOTECAS --------------------



// ---------------- ARQUIVOS INCLUSOS ------------------



#include "robo.h"



// -------------------- CONSTANTES --------------------



// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------

// ------------------ FUNÇÕES -------------------

// Construtor
//Robo::Robo(){


//}

//Destrutor
//Robo::~Robo(){



//}

// Set
void Robo::setPosicao(float x, float y, float theta){

   this->posicao.x =  x;
   this->posicao.y =  y;
   this->posicao.theta = theta;

}
void Robo::setObjetivo(float x, float y, float theta){

   this->objetivo.x = x;
   this->objetivo.y = y;
   this->objetivo.theta = theta;

}
void Robo::setTrajetoria(float x, float y, float theta, int pos){
  
   this->trajetoria.pontos[pos].x = x;
   this->trajetoria.pontos[pos].y = y;
   this->trajetoria.pontos[pos].theta = theta;
 
}
void Robo::setVelocidade(float linear, float angular){

   this->velocidade.linear.data = linear;
   this->velocidade.angular.data = angular;

}
void Robo::setVelDesejada(float linear, float angular){

   this->vel_desejada.linear.data = linear;
   this->vel_desejada.angular.data = angular;

}


// Get
geometry_msgs::Pose2D Robo::getPosicao(){

   return this->posicao;

}
geometry_msgs::Pose2D Robo::getObjetivo(){

   return this->objetivo;

}
estrategia::velocidade Robo::getVelocidade(){

   return this->velocidade;

}
estrategia::velocidade Robo::getVelDesejada(){

   return this->vel_desejada;

}
estrategia::trajetoria Robo::getTrajetoria(){

   return this->trajetoria;

}