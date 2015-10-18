/*
Autobotz UFMG
Desenvolvido por Gustavo Martins Domingues 
2015
(31) 9405-5012
gustdomar@gmail.com

*/



// -------------------- BIBLIOTECAS --------------------

#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


// ---------------- ARQUIVOS INCLUSOS ------------------



#include "estrategia/trajetoria.h"
#include "estrategia/velocidade.h"




// -------------------- CABEÇALHOS --------------------


#ifndef _ROBO_INCLUDED
#define _ROBO_INCLUDED

 class Robo 
   {
   private:
      int blocos_vermelhos;
      int blocos_amarelos;
      int lado_arena; // -1 indica porto, 1 indica plataforma
      geometry_msgs::Pose2D posicao;
      geometry_msgs::Pose2D objetivo;
      estrategia::velocidade velocidade_braco; 
      estrategia::velocidade velocidade_barco;
      estrategia::velocidade velocidade_garra;
      estrategia::trajetoria trajetoria;



   public:
      // Construtor e Destrutor
      //Robo();
      // ~Robo();
      // Set
      void addBlocoVermelho();
      void addBlocoAmarelo();
      void setPosicao(float x, float y, float theta);
      void setObjetivo(float x, float y, float theta);
      void setVelocidadeBarco(float linear, float angular);
      void setVelocidadeBraco(float linear, float angular);
      void setVelocidadeGarra(float linear, float angular);
      void setTrajetoria(float x, float y, float theta);
      
      // Get
      int getBlocosVermelhos();
      int getBlocosAmarelos();
      int getLadoArena();
      geometry_msgs::Pose2D getPosicao();
      geometry_msgs::Pose2D getObjetivo();
      estrategia::velocidade getVelocidade();
      estrategia::velocidade getVelocidadeBarco();
      estrategia::velocidade getVelocidadeGarra();
      estrategia::velocidade getVelocidadeBraco();
      estrategia::trajetoria getTrajetoria();

      // outros
      void limpaTrajetoria();
      void incializa();
   };


#endif