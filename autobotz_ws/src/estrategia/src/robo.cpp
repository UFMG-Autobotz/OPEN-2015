/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

Descrição do arquivo
*/



// -------------------- BIBLIOTECAS --------------------

#include <geometry_msgs/Pose2D.h>

// ---------------- ARQUIVOS INCLUSOS ------------------



#include "robo.h"


#ifndef INCLUIR
#define INCLUIR

#define ARENA_X 400 
#define ARENA_Y 200

#define VEL_LIN_MAX 255
#define VEL_LIN_MIN -255
#define VEL_ANG_MAX 255
#define VEL_ANG_MIN -255



#endif


// -------------------- CONSTANTES --------------------



// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------

// ------------------ FUNÇÕES -------------------

// Construtor
/*Robo::Robo(){



}*/

//Destrutor
//Robo::~Robo(){



//}

// Set

void Robo::addBlocoVermelho(){

   this->blocos_vermelhos += 1;

}
void Robo::addBlocoAmarelo(){

    this->blocos_amarelos += 1;

}
void Robo::setPosicao(float x, float y, float theta){

   this->posicao.x =  x;
   this->posicao.y =  y;
   this->posicao.theta = theta;

   if (this->posicao.x >= ARENA_X/2)
      this->lado_arena = 1; // porto
   else
      this->lado_arena = -1; // plataforma

}
/*void Robo::setObjetivo(float x, float y, float theta){

   this->objetivo.x = x;
   this->objetivo.y = y;
   this->objetivo.theta = theta;

}*/
void Robo::addPontoTrajetoria(float x, float y, float theta){
  
   geometry_msgs::Pose2D posicao;
   posicao.x = x;
   posicao.y = y;
   posicao.theta = theta;
   this->trajetoria.pontos.push_back(posicao);
 
}
void Robo::setVelocidadeBarco(float linear, float angular){

   this->velocidade_barco.linear.data = linear;
   this->velocidade_barco.angular.data = angular;

   if (linear > VEL_LIN_MAX)
      this->velocidade_barco.linear.data = VEL_LIN_MAX;
   if (angular > VEL_LIN_MAX)
      this->velocidade_barco.angular.data = VEL_ANG_MAX;

   if (linear < VEL_LIN_MIN)
      this->velocidade_barco.linear.data = VEL_LIN_MIN;
   if (angular < VEL_LIN_MIN)
      this->velocidade_barco.angular.data = VEL_ANG_MIN;

}
void Robo::setVelocidadeBraco(float linear, float angular){

   // como a velocidade sera usada também como pwm, deve-se limitá-la por 255
   this->velocidade_braco.linear.data = linear;
   this->velocidade_braco.angular.data = angular;

   if (linear > VEL_LIN_MAX)
      this->velocidade_braco.linear.data = VEL_LIN_MAX;
   if (angular > VEL_LIN_MAX)
      this->velocidade_braco.angular.data = VEL_ANG_MAX;

   if (linear < VEL_LIN_MIN)
      this->velocidade_braco.linear.data = VEL_LIN_MIN;
   if (angular < VEL_LIN_MIN)
      this->velocidade_braco.angular.data = VEL_ANG_MIN;

}

void Robo::setVelocidadeGarra(float linear, float angular){

   this->velocidade_garra.linear.data = linear;
   this->velocidade_garra.angular.data = angular;

   if (linear > VEL_LIN_MAX)
      this->velocidade_garra.linear.data = VEL_LIN_MAX;
   if (angular > VEL_LIN_MAX)
      this->velocidade_garra.angular.data = VEL_ANG_MAX;

   if (linear < VEL_LIN_MIN)
      this->velocidade_garra.linear.data = VEL_LIN_MIN;
   if (angular < VEL_LIN_MIN)
      this->velocidade_garra.angular.data = VEL_ANG_MIN;

}

// Get

int Robo::getBlocosVermelhos(){

   return this->blocos_vermelhos;

}
int Robo::getBlocosAmarelos(){

   return this->blocos_amarelos;

}

int Robo::getLadoArena(){

   return this->lado_arena;

}
geometry_msgs::Pose2D Robo::getPosicao(){

   return this->posicao;

}
/*geometry_msgs::Pose2D Robo::getObjetivo(){

   return this->objetivo;

}*/
estrategia::velocidade Robo::getVelocidadeBarco(){

   return this->velocidade_barco;

}
estrategia::velocidade Robo::getVelocidadeBraco(){

   return this->velocidade_braco;

}
estrategia::velocidade Robo::getVelocidadeGarra(){

   return this->velocidade_garra;

}
estrategia::trajetoria Robo::getTrajetoria(){

   return this->trajetoria;

}

// Outros

void Robo::limpaTrajetoria(){

   this->trajetoria.pontos.clear();

}

void Robo::zeraAtributos(){


   this->blocos_vermelhos = 0;
   this->blocos_amarelos = 0;
   this->lado_arena = -1;  // -1 indica porto, 1 indica plataforma
   this->trajetoria.pontos.clear();
   
   // TESTES
   //this->setObjetivo(370.0, 150.0, 0.0);


}