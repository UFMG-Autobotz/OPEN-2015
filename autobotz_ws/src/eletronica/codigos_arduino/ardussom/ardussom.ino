/*

= {3,4,5,6}; // Echo Pin 
 = {7,8,9,10}; // Trigger Pin
 int echoPin[5];
int trigPin[5];
   

 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.

roscore
rosrun rosserial_python serial_node.py /dev/ttyACM0
rostopic echo /eletronica/ultrassom/D

 */

#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#define INF 60000

ros::NodeHandle  nh;
std_msgs::Float32 distanciaF;
ros::Publisher chatterF("/eletronica/ultrassom/raw/F", &distanciaF);
void messageCb(const std_msgs::Int32& msg);
const char* ROS_stepper_topic = "/eletronica/base/stepper";
ros::Subscriber<std_msgs::Int32> sub(ROS_stepper_topic, &messageCb);

/*************** Variaveis do ULTRASSOM ************************/
int echoPin[4];  // Echo Pin
int trigPin[4];  // Trigger Pin
unsigned long lastDisplay;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed

/*************** Variaveis do Stepper **************************/
const int stepper_pins[] = {8, 9, 10, 11}; //pinos nos quais esta o stepper
                                          //Os pinos sao ativados na ordem
                                          //desse vetor

const int T_stepper_min = 100;  //menor periodo (|vel| = 255) em ms
                       //1 periodo = ciclo de ativacao dos 4 pinos
const int T_stepper_max = 1000;

int T_stepper_atual = 0; //periodo a ser setado a partir do topico do ROS
int dir;      //direcao do stepper 1 = frente, -1 = tras

int T_ROS_spin = 30;  //periodo em ms entre uma chamada do ROS e outra

/******************** PROCESSAMENTO DO STEPPER ******************/
//callback para pegar o valor de velocidade no topico
void messageCb(const std_msgs::Int32& msg)
{
	int vel = msg.data;
	
	if(vel > 0)
	{
		dir = 1;
		if(vel >255)
			vel =  255;
	}
	else if(vel < 0) 
	{
		dir = -1;
		if(vel < -255)
			vel = -255;
	}
	
	if(vel == 0)
		T_stepper_atual = INF;
	else
		T_stepper_atual = ( 1 - float(abs(vel))/255 )*(T_stepper_max - T_stepper_min) + T_stepper_min;  //faz com que o periodo seja uma rampa linear
}

////////////////////    parte do stepper    ////////////////////

void step_forward(int& indice)
{
	int proximo;
	if(indice >= 3)
        {
		proximo = stepper_pins[0];
                indice = 0;
        }
	else
	{
		proximo = stepper_pins[indice + 1];
                indice++;
        }
        
	for(int i = 0; i < 4; i++)
	  digitalWrite(stepper_pins[i], LOW);
	
        digitalWrite(proximo, HIGH);
 
        digitalWrite(13, HIGH - digitalRead(13));
}

void step_back(int & indice)
{
	int proximo;
	if(indice <= 0)
    {
		proximo = stepper_pins[3];
        indice = 3;
    }
	else
	{
		proximo = stepper_pins[indice - 1];
        indice--;
    }

	for(int i = 0; i < 4; i++)
		digitalWrite(stepper_pins[i], LOW);
	digitalWrite(proximo, HIGH);
}

void step()   //faz um passo do stepper
{	
    static int indice = 0;   
  
    if(dir > 0)
      step_forward(indice);
    else if(dir < 0)
      step_back(indice);
}

//variaveis para temporizacao
unsigned long time_start_stepper;
unsigned long time_start_ROS;
/***************************************************************/

void setup() {  
  nh.initNode();
  nh.advertise(chatterF);
  nh.subscribe(sub);
  
  for(int i = 10; i < 14; i++)
  {
        pinMode(stepper_pins[i], OUTPUT);
        digitalWrite(stepper_pins[i], LOW);
  }
  
  for(int i=0; i<4; i++){ 
  echoPin[i] = (i+1)*2; 
  trigPin[i] = (i*2) + 3; 
  }

  //setup pins
  for(int i=0; i<4; i++){ 
  pinMode(trigPin[i], OUTPUT);
  pinMode(echoPin[i], INPUT);
  }
  
  time_start_stepper = micros();
  time_start_ROS = micros();
  
}

void getDistance(float* duration, float* distanceCm, int num)
{
 digitalWrite(trigPin[num], LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin[num], HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin[num], LOW);
 *duration = pulseIn(echoPin[num], HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 *distanceCm = *duration/58.2;
 
 if (*distanceCm >= maximumRange)
   *distanceCm = -1;
 if (*distanceCm <= minimumRange)
   *distanceCm = -2;
}


void loop()
{
 if((micros() - time_start_ROS)/1000 >= T_ROS_spin){
    nh.spinOnce();
    time_start_ROS = micros();
 }
 	
//se já passou um quarto de periodo
 if((micros() - time_start_stepper)/1000 >= T_stepper_atual/4){
    step();
    time_start_stepper = micros();
 }
  
 float duration, distance; 
 
 getDistance(&duration, &distance,2);
 distanciaF.data = distance;
 delay(240);
 chatterF.publish( &distanciaF );
 
 nh.spinOnce();
}