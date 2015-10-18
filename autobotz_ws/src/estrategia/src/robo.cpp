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



// -------------------- CONSTANTES --------------------

#define ARENA_X 500
#define ARENA_Y 300

// ----------------- VARIÁVEIS GLOBAIS ------------------



// ---------------- CALLBACK FUNCTIONs -----------------

// ------------------ FUNÇÕES -------------------

// Construtor
Robo::Robo(){
   this->blocos_vermelhos = 0;
   this->blocos_amarelos = 0;
   this->lado_arena = -1;  // -1 indica porto, 1 indica plataforma
   this->trajetoria.pontos.clear();
}

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
void Robo::setObjetivo(float x, float y, float theta){

   this->objetivo.x = x;
   this->objetivo.y = y;
   this->objetivo.theta = theta;

}
void Robo::setTrajetoria(float x, float y, float theta){
  
   geometry_msgs::Pose2D posicao;
   posicao.x = x;
   posicao.y = y;
   posicao.theta = theta;
   this->trajetoria.pontos.push_back(posicao);
 
}
void Robo::setVelocidadeBarco(float linear, float angular){

   this->velocidade_barco.linear.data = linear;
   this->velocidade_barco.angular.data = angular;

}
void Robo::setVelocidadeBraco(float linear, float angular){

   this->velocidade_braco.linear.data = linear;
   this->velocidade_braco.angular.data = angular;

}

void Robo::setVelocidadeGarra(float linear, float angular){

   this->velocidade_garra.linear.data = linear;
   this->velocidade_garra.angular.data = angular;

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
geometry_msgs::Pose2D Robo::getObjetivo(){

   return this->objetivo;

}
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