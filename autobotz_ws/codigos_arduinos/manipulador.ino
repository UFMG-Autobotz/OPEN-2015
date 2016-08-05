///////////////////////////////////////////////////////////////////////////////
//  Part of Autobotz' OPEN-2015 robotic boat
//
//  The Arduinos in the boat are wired in an I2C bus, this is one is 
//  responsible for the manipulator in the robot.
//	The following modules are onnected to this Arduino:
//    - Base giratoria
//    - Manipulador japones (caracol)
//    - Manipulador japones (garra)
//
//  This code uses the Wirebotz library, v_1.0. Make sure it is in your IDE's
//  libraries folder
//


///////////////////////////////////////////////////////////////////////////////
//  Part of Autobotz' OPEN-2015 robotic boat
//
//  The Arduinos in the boat are wired in an I2C bus, this is one is 
//  responsible for the manipulator in the robot.
//	The following modules are onnected to this Arduino:
//    - Base giratoria
//    - Manipulador japones (caracol)
//    - Manipulador japones (garra)
//
//  This code uses the Wirebotz library, v_1.0. Make sure it is in your IDE's
//  libraries folder
//

#include <Wire.h>
#include <WireBotzSlave.h>

///////////////////  Base Giratória  //////////////////////
// The motor has 48 steps per turn. This arduino gets 
// messages telling to what position it should go.
//
// We assume that the initial position is 0. There
// is currently no sensor to know the real position.
#define clockwise         0     //define turning directions
#define counterClockwise  1    //
#define OFF 255   //turn off all coils on the stepper

#define stepsPerTurn 48

byte stepperPins[4] = {1, 1, 1, 1};  //when pins are activated in this order, the base turns clockwise
byte pulseTime = 50;   //time of one pulse (a stepper cycle has 4 pulses), ms
byte direction;        //turning direction

byte basePosition;   //current pos
byte baseGoal;

unsigned long lastStep;  //time of last step

void step(byte dir);
//produces one step in the specified direction   
//
//this call only does the transition from one step to the
//next, timing must be handled outside it

///////////////////     Caracol      //////////////////////
///////////////////      Garra       //////////////////////

///////////////////  I2C parameters  //////////////////////
#define RX_SIZE 4   //size of RX buffer
#define TX_SIZE 4   //size of TX buffer

#define ADDRESS 0b00001101   //our address (13)

byte RXBuff[RX_SIZE];  //buffers to receive and send I2C data
byte TXBuff[TX_SIZE];


///////////////////  setup() and loop()  //////////////////

void setup()
{
	//setup stepper
	basePosition = 0;   //no position sensor implemented, assuming 0
	baseGoal = 0;
	direction = OFF;

	for(int i = 0; i < 4; i++)
	{
		pinMode(stepperPins[i], OUTPUT);
		digitalWrite(stepperPins[i], 0);
	}

	//setup caracol

	//setup garra

	//setup I2C
	Slave.begin(ADDRESS);
	Slave.setRXBuffer(RXBuff, RX_SIZE);
	Slave.setTXBuffer(TXBuff, TX_SIZE);	

	//indicator LED
	pinMode(13, OUTPUT);

	//timers
	lastStep = micros();
}

void loop()
{
	//rodar base giratoria
	if( (micros() - lastStep >= pulsese*1000 )
	{
		if(direction == counterClockwise && basePosition > baseGoal)
		{
			basePosition = basePosition - 1;
			step(direction);
		}
		else if(direction == clockwise && basePosition < baseGoal)
		{
			basePosition = basePosition + 1;
			step(direction);
		}

		lastStep = micros();
	}

	//caracol
	//TODO: código do caracol aqui

	//garra
	//TODO: código da garra aqui

	//update rx and tx buffers
	if(Slave.newMessage())
	{
		if(RXBuff[0] == 1)   //stepper message
		{
			//first byte
			direction = RXBuff[1];

			//second byte
			baseGoal = RXBuff[2];
			if(baseGoal >= stepsPerTurn)
				baseGoal = stepsPerTurn-1;

			//third byte
			pulseTime = RXBuff[3];
		}
		else if(RXBuff == 2)  //caracol message
		{
			//TODO: decodifica menssagem de controle do caracol
		}
		else if(RXBuff == 3)  //garra message
		{
			//TODO: decodifica menssagem de controle da garra
		}
	}

	//TODO: update txBuffer from sensor readings

	//altos nadas
	delay(10);
}

///////////////////  Function implementations  ////////////

void step(int dir)
{
	static byte index = 0;

	if(dir == OFF)
	{
		digitalWrite(stepperPins[0], 0);
		digitalWrite(stepperPins[1], 0);
		digitalWrite(stepperPins[2], 0);
		digitalWrite(stepperPins[3], 0);
		return;
	}
	else if(dir == counterClockwise)
		index++;
	else  //clockwise
		index--;

	if(index == 4) index = 0;  //limit index values to 0 ~ 3

	for(int i = 0; i < 4; i++)
		digitalWrite(stepperPins[i], i == index);  //write the new state
}
