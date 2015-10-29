#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#define incr 5
using namespace std;

int main(int argc, char **argv){

  struct termios old_tio, new_tio;

  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);

  char aux[20], c;
  int pwm1 = 0,pwm2 = 0;
  int pwm1a = 0,pwm2a = 0;

int pwm3 = 0,pwm4 = 0, pwm5 = 0;
  int pwm3a = 0,pwm4a = 0;

  int robo = 1, count = 0;
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("/eletronica/base/stepper", 1000);
ros::Publisher chatter_pubMR = n.advertise<std_msgs::Int32>("/controle/braco/motor/R", 1000);
ros::Publisher chatter_pubML = n.advertise<std_msgs::Int32>("/controle/braco/motor/L", 1000);

ros::Publisher chatter_pubG = n.advertise<std_msgs::Int32>("/controle/garra/motor", 1000);

ros::Publisher ultrassomF = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/raw/F", 1000);
ros::Publisher ultrassomL = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/raw/L", 1000);

ros::Publisher Stp_ativo = n.advertise<std_msgs::Bool>("/eletronica/stepper/ativado", 1000);


  cout<<"RÁDIO CONTROLE - OPEN 2015"<<endl;
  while (ros::ok()){
    
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    std_msgs::Int32 msg, msgM, msgG;
    std_msgs::Float32 ultraL, ultraF, ultraR, ultraB;
    std_msgs::Bool stepper;
    fflush(stdin);
    c = getchar();
    fflush(stdin);


    //interpreta teclas
    if(c == 'w'){
	pwm1+=incr;
	pwm2+=incr;
	if(pwm1 > 255) pwm1 = 255;
	if(pwm2 > 255) pwm2 = 255;
	stepper.data = true;
    } 
    else if (c == 's'){
	pwm1= pwm1 - incr;
	pwm2= pwm2 - incr;
	if(pwm1 < -255) pwm1 = -255;
	if(pwm2 < -255) pwm2 = -255;
	stepper.data = true;
    }

    else if (c == 'd'){
	pwm3-=incr;
	pwm4+=incr;
	if(pwm3 < -255) pwm3 = -255;
	if(pwm4 > 255) pwm4 = 255;
    }
    else if (c == 'a'){
	pwm3+=incr;
	pwm4-=incr;
        if(pwm3 > 255) pwm3 = 255;
	if(pwm4 < -255) pwm4 = -255;	
    }

    else if (c == 'e'){
	pwm5 = 1;
    }

 else if (c == 'q'){
	pwm5 = -1;	
    }
     
//else if (c == 'e'){
//	pwm2+=incr;
//	if(pwm2 > 255) pwm2 = 255;
//    }
//    else if (c == 'q'){
//	pwm1+=incr;
//        if(pwm1 > 255) pwm1 = 255;
//    }
    else if (c == 'c'){
	pwm2-=incr;
	if(pwm2 < -255) pwm2 = -255;
    }
    else if (c == 'z'){
	pwm1-=incr;
        if(pwm1 < -255) pwm1 = -255;
    }

    else if(c == 'r'){
        pwm1 = pwm2 = 0;
	 pwm3 = pwm4 = 0;
        pwm5 = 0;
	stepper.data = false;
    }

    //muda o robo que esta sendo controlado
	else if(c == '1'){ robo = 1; pwm1 = pwm2 = 0;}
	else if(c == '2'){ robo = 2; pwm1 = pwm2 = 0;}
	else if(c == '3'){ robo = 3; pwm1 = pwm2 = 0;}
	else if(c == '4'){ robo = 4; pwm1 = pwm2 = 0;}
	else if(c == '5'){ robo = 5; pwm1 = pwm2 = 0;}
	else if(c == '6'){ robo = 6; pwm1 = pwm2 = 0;}

    //traduz dados para string

	pwm1a = pwm1;
	pwm2a = pwm2;
	pwm3a = pwm3;
	pwm4a = pwm4;

//	if(pwm1a < 0){
//		pwm1a = pwm1a*(-1);
//		pwm1a = pwm1a + 255;
//		if (pwm1a > 510) pwm1a = 510;
//		cout<<"PWM 1:  "<< ((pwm1a-255)*-1)<<"  ";
//		cout<<"tras   |  ";
//	}
//	else{
//		cout<<"PWM 1:  "<< pwm1a<<"  ";
//		cout<<"frente   |  ";}

//	if(pwm2a < 0){
//		pwm2a = pwm2a*(-1);
//		pwm2a = pwm2a + 255;
//		if (pwm2a > 510) pwm2a = 510;
//		cout<<"PWM 1:  "<< ((pwm2a-255)*-1)<<"  ";
//        	cout<<"tras   ";
//	}
//	else {
//		cout<<"PWM 2:  "<< pwm2a<<"  ";
//		cout<<"frente ";
//	}	
	
    cout<<endl;

//    sprintf(aux,"BA%i%.3i%.3iB",robo, pwm1a, pwm2a); 
// 
    if(count == 1){
	count = 0;
    }
    cout<<"PWM 1:  "<< pwm1a;
    msg.data = pwm2a;
    
	
    cout<<"   Motores (caracol):  "<< pwm4a;
    cout<<"   Garra: "<<pwm5<<"  ";

if(stepper.data == true)
cout << "Estado_stepper: 1";
if(stepper.data == false)
cout << "Estado_stepper: 0";

    msgG.data = pwm5;
    msgM.data = pwm4a;
    chatter_pub.publish(msg);
    chatter_pubG.publish(msgG);
    chatter_pubMR.publish(msgM);
    chatter_pubML.publish(msgM);
    Stp_ativo.publish(stepper);

//    ultraL.data = (float)pwm1a;
//    ultraF.data = (float)pwm1a;

//    ultrassomL.publish(ultraL);
//    ultrassomF.publish(ultraF);
//    cout<<"   UltrassomF "<<ultraF.data<<" UltrassomL "<<ultraL.data<<endl;

    ros::spinOnce();
    count ++;
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio); 
  }
	
  return 0;
}