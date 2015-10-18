// Esse codigo roda no arduino e controla a ativação de um motor 
// de passo a partir de um topico no ROS
//
// É considerado que o motor está conectado aos pinos P*
// e que a informacao de rotacao esta sendo publicada no topico
// "/eletronica/base/stepper" como um inteiro cujo valor vai
// de -255 a 255, onde 0 é parado

#include <ros.h>
#include <std_msgs/Int32.h>

#define INF 60000

/////// parametros do stepper

const int stepper_pins[] = {8, 9, 10, 11}; //pinos nos quais esta o stepper
                                          //Os pinos sao ativados na ordem
                                          //desse vetor
unsigned char indice;  //posicao atual no vetor de pinos

const int T_min = 100;  //menor periodo (|vel| = 255) em ms
                       //1 periodo = ciclo de ativacao dos 4 pinos
const int T_max = 1000;

/////// vairaveis para o ROS
ros::NodeHandle nh;
void messageCb(const std_msgs::Int32& msg);
const char* ROS_stepper_topic = "/eletronica/base/stepper";
ros::Subscriber<std_msgs::Int32> sub(ROS_stepper_topic, &messageCb);

int T_atual = 0; //periodo a ser setado a partir do topico do ROS
int dir;      //direcao do stepper

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
		T_atual = INF;
	else
		T_atual = ( 1 - float(abs(vel))/255 )*(T_max - T_min) + T_min;  //faz com que o periodo seja uma rampa linear
}

void step()   //faz um passo do stepper
{	
	unsigned char atual = stepper_pins[indice];
	unsigned char proximo;
	if(indice == 3)
		proximo = stepper_pins[0];
	else
		proximo = stepper_pins[indice+1];
	
	digitalWrite(atual, LOW);
	digitalWrite(proximo, HIGH);

        indice++;
}

unsigned long time_start;

void setup()
{
	for(int i = 0; i < 3; i++)
		pinMode(stepper_pins[i], OUTPUT);
	
	nh.initNode();
	nh.subscribe(sub);

        time_start = micros();

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
}

 
void loop()
{
 	nh.spinOnce();
 	
	//se já passou um quarto de periodo
	if((micros() - time_start)/1000 >= T_atual/4)
 	{
           digitalWrite(13, HIGH - digitalRead(13));
 		step();
 		time_start = micros();
 	}		
 	
}
 
