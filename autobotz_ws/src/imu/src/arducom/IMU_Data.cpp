// ROS
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <stdlib.h>
#include "rs232.h"

// C++
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include<string.h>

//Timer
#include <unistd.h>
#include <ctime>

//variaveis
int bdrate = 115200, debug = 1;
int serialPort; //porta serial de comunicação

//Salvar em um Arquivo
#include <iostream>
#include <unistd.h>
#include <fstream> 
#include <string>

using namespace std;

int main(int argc, char **argv){
 
  std::ofstream IMU("Salva_dados_IMU.txt");
  if (!IMU)
  {
        // Print an error and exit
        cerr << "O arquivo IMUread.cpp não foi encontrado!" << endl;
        ROS_INFO("O arquivo IMUread.cpp não foi encontrado!\n\n\n");
	exit(1);
  }

  char texto[20]; 
  int size = 7;

  if(!argv[1]){
    ROS_INFO("\n\nUse: rosrun nome_do_pacote arducom [port number]\nVide port number in: http://www.teuniz.net/RS-232/\nSerial ACM0 => 24,  Serial ACM1 => 25 ...\n\n");
    return -1;
  }
	 
  //tenta abrir a comunicação serial
  //serial == 24 ou 25 (ACM0 ou ACM1) "/dev/ttyACM0"

  serialPort = atoi(argv[1]);

  if(RS232_OpenComport(serialPort, bdrate)){
	ROS_INFO("Can not open comport\n");
	return(0);
  }
  else 
	ROS_INFO("Arduino conected!\n");

  if(argv[2]) {
	debug = 1;
  }

  ros::init(argc, argv, "IMUSerial"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Float32>("/eletronica/imu/yaw", 1000);
  ros::Rate loop_rate(10);

  IMU <<"Autobotz UFMG, Jul/2015."<<endl;
  IMU <<"\n\nEsse documento salva os dados referentes a orientação(yaw) da IMU MPU9150!\n\n"<< endl;

  time_t now = time(0);// current date/time based on current system
  char* dt = ctime(&now);// convert now to string form
  ROS_INFO(" %s \n", dt);
  double cont = 0;
  IMU << dt <<endl;

  float yaw;
  
  while(ros::ok()){
    int n;
    int s = strlen (texto);
    std_msgs::Float32 msg;

    if(size>4096)  size = 4096;
    n = read(Cport[serialPort], texto, size);

    if(s == 7){
	IMU << texto <<endl;
	
           
	yaw = atof(texto);
        msg.data = yaw;
        chatter_pub.publish(msg);
	ROS_INFO("Direcao =  %f\n ", yaw);

        cont++;
	if(cont==50){
	time_t now = time(0);
	char* dt = ctime(&now);// convert now to string form
  	ROS_INFO("**   %s   **\n", dt);
	IMU << "\n" << dt <<endl;
	cont = 0;	
	}
    }
    else 
	ROS_INFO("Erro! \n"); 
    
    ros::spinOnce();
    loop_rate.sleep();
    usleep(100 * 1000);
  }

  IMU.close();
  ros::spin();
  return 0;

}
