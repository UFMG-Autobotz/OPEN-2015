// Esse codigo roda no arduino e controla a ativação de um motor 
// de passo a partir de um topico no ROS
//
// É considerado que o motor está conectado aos pinos P*
// e que a informacao de rotacao esta sendo publicada no topico
// "/eletronica/base/stepper" como um inteiro cujo valor vai
// de -255 a 255, onde 0 é parado
//
// Para rodar: rosrun rosserial_python serial_node.py /dev/ttyACM2

#include <ros.h>
#include <std_msgs/Int32.h>

#define INF 60000

/////// parametros do stepper

const int stepper_pins[] = {8, 9, 10, 11}; //pinos nos quais esta o stepper
                                          //Os pinos sao ativados na ordem
                                          //desse vetor

const int T_stepper_min = 100;  //menor periodo (|vel| = 255) em ms
                       //1 periodo = ciclo de ativacao dos 4 pinos
const int T_stepper_max = 1000;

int T_stepper_atual = 0; //periodo a ser setado a partir do topico do ROS
int dir;      //direcao do stepper 1 = frente, -1 = tras

int T_ROS_spin = 30;  //periodo em ms entre uma chamada do ROS e outra

////////////////      parte do ROS    ///////////////////
ros::NodeHandle nh;
void messageCb(const std_msgs::Int32& msg);
const char* ROS_stepper_topic = "/eletronica/base/stepper";
ros::Subscriber<std_msgs::Int32> sub(ROS_stepper_topic, &messageCb);


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

void setup()
{	
    for(int i = 0; i < 4; i++)
    {
        pinMode(stepper_pins[i], OUTPUT);
        digitalWrite(stepper_pins[i], LOW);
    }

	nh.initNode();
	nh.subscribe(sub);

    time_start_stepper = micros();
	time_start_ROS = micros();

    pinMode(13, OUTPUT);     //for debug only
    digitalWrite(13, HIGH);  // 
}


void loop()
{
	if((micros() - time_start_ROS)/1000 >= T_ROS_spin)
	{
 		nh.spinOnce();
 		time_start_ROS = micros();
	}
 	
	//se já passou um quarto de periodo
	if((micros() - time_start_stepper)/1000 >= T_stepper_atual/4)
 	{
 		step();
 		time_start_stepper = micros();
 	}
 	
 	delay(1);
}

