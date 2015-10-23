// ROS
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <stdlib.h>
#include "rs232.h"

// C++
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

//variaveis
int bdrate = 115200, debug = 1;
int serialPort; //porta serial de comunicação

//Salvar em um Arquivo
#include <iostream>
//#include <unistd.h>
//#include <fstream> 
#include <string>

using namespace std;

int main(int argc, char **argv){
 
  int size = 7;   //tamanho da string enviada pelo arduino
  const int TX_SIZE = 20;
  unsigned char tx[TX_SIZE];

  //conferir se tem argv[1]
  if(argc < 2){
    cout << "\nNumero insuficiente de argumentos!" << endl;
    cout << "Uso: rosrun nome_do_pacote arducom [port name]" << endl;
    return -1;
  }
	
  //converte nome de porta serial para numero
  string nome_da_porta = argv[1];
  int serialPort;

  if(nome_da_porta == "/dev/ttyACM0")
    serialPort = 24;
  else if(nome_da_porta == "/dev/ttyACM1")
    serialPort = 25;
  else if(nome_da_porta == "/dev/ttyACM2")
    serialPort = 26;
  else if(nome_da_porta == "/dev/ttyACM3")
    serialPort = 27;
  else if(nome_da_porta == "/dev/ttyACM4")
    serialPort = 28;
  else if(nome_da_porta == "/dev/ttyACM5")
    serialPort = 29;
  else if(nome_da_porta == "/dev/ttyACM6")
    serialPort = 30;
  else
  {
    cout << "\nO arquivo \"" << argv[1] <<"\" nao esta identificado com uma porta serial." << endl;
    cout << "\nSe esse arquivo realmente e o endereco correto da porta serial" << endl; 
    cout << "associada ao arduIMU, adicione no codigo fonte de arducom\n" <<endl;

    return -2;
  }

  //tenta abrir a comunicação serial
  if(RS232_OpenComport(serialPort, bdrate)){
	ROS_INFO("Can not open COM port\n");
	return(0);
  }
  else 
	ROS_INFO("Arduino conected!\n");

  if(argv[2]) {
	debug = 1;
  }

  //abrir comunicacao com o ROS
  ros::init(argc, argv, "IMUSerial"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Float32>("/eletronica/imu/yaw", 1000);
  ros::Rate loop_rate(3);
  float yaw;

  int tam = RS232_PollComport(serialPort,tx,size);
  char* texto = reinterpret_cast<char*>(tx);
  
  while(tam>0){
     tam = RS232_PollComport(serialPort,tx,size);
     cout<<"Tam: "<<tam<<endl;
  }
  int s;

  while(ros::ok()){

    int n;
    std_msgs::Float32 msg;

    //ler menssagem da porta serial
    memset(tx, '\0', sizeof(char)*TX_SIZE);  //enche tx de '\0'
    RS232_PollComport(serialPort,tx,size);   //le da porta serial

    char* texto = reinterpret_cast<char*>(tx);
    s = strlen(texto);

    if(s == 7){
	yaw = atof(texto);
        msg.data = yaw;
        chatter_pub.publish(msg);
	cout<<"Direcao: "<<yaw<<endl;
    }
    else
    {
	cout<<"Erro! Recebeu menssagem de tamanho " << s << endl;
	//cout<<"Menssagem: " << tx << endl;
    } 
    
    //dormir para manter a frequencia do loop
    ros::spinOnce();
    loop_rate.sleep();
    //usleep(200 * 1000);
  }

  ros::spin();
  return 0;

}
