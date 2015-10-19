/*
Esse programa será usado para obter o tempo entre o envio de um pacote e outro.

Echo Pin = {3,4,5,6};  
Trigger Pin = {7,8,9,10};  
int echoPin[5];
int trigPin[5];
   
Desse jeito o delta fica entre 109 e 113 ms
Caso todos os ultrassônicos não estejam conectados o pulsein esperará até um certo instante e depois retornará, ou seja, conecte todos os ultrassonicos caso queira testar o tempo.

roscore
rosrun rosserial_python serial_node.py /dev/ttyACM0
rostopic echo /eletronica/ultrassom/D
*/

int echoPin[4];  // Echo Pin
int trigPin[4];  // Trigger Pin

unsigned long lastDisplay;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
float distanciaD, distanciaE, distanciaF, distanciaT;

void setup() {  
  Serial.begin(115200);
  
  for(int i=0; i<4; i++){ 
  echoPin[i] = (i+1)*2; 
  trigPin[i] = (i*2) + 3; 
  }
  //setup pins
  for(int i=0; i<4; i++){ 
  pinMode(trigPin[i], OUTPUT);
  pinMode(echoPin[i], INPUT);
  }

  lastDisplay =  millis();
}

void getDistance(float* duration, float* distanceCm, int num)
{
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin[num], LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin[num], HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin[num], LOW);
 *duration = pulseIn(echoPin[num], HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 *distanceCm = *duration/58.2;
 
 if(*distanceCm <= minimumRange)
     *distanceCm = -1;
 
 if (*distanceCm >= maximumRange)
     *distanceCm = -2; 
}


void loop()
{
 unsigned long now = millis();
 float duration, distance; //sensor reads distance
 
 getDistance(&duration, &distance,0);
 distanciaD = distance; 
 
 getDistance(&duration, &distance,1);
 distanciaE = distance;
 
 getDistance(&duration, &distance,2);
 distanciaF = distance; 
 
 getDistance(&duration, &distance,3);
 distanciaT = distance; 
 
 Serial.print("Delta: ");
 Serial.println(now - lastDisplay);
 lastDisplay = now;

 Serial.print("Distance D: ");
 Serial.print(distanciaD);
 Serial.print("  Distance E: ");
 Serial.print(distanciaE);
 Serial.print("  Distance F: ");
 Serial.print(distanciaF);
 Serial.print("  Distance T: ");
 Serial.println(distanciaT);
 
 delay(50);
}
