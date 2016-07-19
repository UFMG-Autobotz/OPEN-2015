///////////////////////////////////////////////////////////////////////////////
//  Part of Autobotz' OPEN-2015 robotic boat
//
//  The Arduinos in the boat are wired in an I2C bus, this is one is called
//  base because it takes care of the actual boat, the base of the robot.
//	The following modules are onnected to this Arduino:
//    - Propellers
//    - Ultrassounds
//
//  This code uses the Wirebotz library, v_1.0. Make sure it is in your IDE's
//  libraries folder
//

#include <Arduino.h>
#include <Wire.h>
#include <WireBotzSlave.h>

///////////////////  Ultrassound parameters  //////////////
byte trigPin[4] = {1, 1, 1, 1};
byte echoPin[4] = {1, 1, 1, 1};
#define maximumRange = 200; // Maximum range (cm)
#define minimumRange = 1;   // Minimum range (cm)

#define outOfRangeValue = 255;

///////////////////  Propeller parameters  ////////////////
#define PWM_L 1
#define DIR_L 1
#define PWM_R 1
#define DIR_R 1

///////////////////  I2c parameters  //////////////////////
#define RX_SIZE 4   //size of RX buffer
#define TX_SIZE 4   //size of TX buffer

#define ADDRESS 0b00001010   //our address

byte RXBuff[RX_SIZE];  //buffers to receive and send I2C data
byte TXBuff[TX_SIZE];

///////////////////  Useful functions  ////////////////////

float getDistance(int num);
//gets distance on the num'th ultrassound


///////////////////  setup() and loop()  //////////////////

void setup()
{
	//setup propellers
	pinMode(PWM_R, OUTPUT);
	analogWrite(PWM_R, 0);
	pinMode(PWM_L, OUTPUT);
	analogWrite(PWM_L, 0);

	pinMode(DIR_R, OUTPUT);
	pinMode(DIR_L, OUTPUT);

	//setup ultrassounds
	for(byte i = 0; i < 4; i++)
	{
		pinMode(trigPin[i], OUTPUT);
		digitalWrite(trigPin[i], 0);
		pinMode(echoPin[i], INPUT);
	}

	//setup I2C
	Slave.setup();
	Slave.setRXBuffer(RXBuff, RX_SIZE);
	Slave.setTXBuffer(TXBuff, TX_SIZE);
}

void loop()
{
	float tmpDist;

	//update propeller state
	if(Slave.newMessage())
	{
		digitalWrite(DIR_L, RXBuff[0]);
		digitalWrite(PWM_L, RXBuff[1]);
		digitalWrite(DIR_R, RXBuff[2]);
		digitalWrite(PWM_R, RXBuff[3]);
	}

	//read from ultrassounds
	for(int i = 0; i < 4; i++)
	{
		tmpDist = getDistance(i);
		TXBuff[i] = (byte) tmpDist;
	}

	//fazer altos nadas
	delay(100);
}

//////////////////  Function implementations  /////////////

float getDistance(int num)
{
	//TODO: reasonable timeout (default on pulseIn is to big)

	digitalWrite(trigPin[num], LOW); 
	delayMicroseconds(2); 

	digitalWrite(trigPin[num], HIGH);
	delayMicroseconds(10); 

	digitalWrite(trigPin[num], LOW);
	unsigned long duration = pulseIn(echoPin[num], HIGH);

	//Calculate the distance (in cm) based on the speed of sound.
	int distanceCm = duration/58.2;

	if (distanceCm > maximumRange || distanceCm < minimumRange)
		distanceCm = outOfRangeValue;

	return distanceCm;
}
