int ledPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  int leituraResistor = analogRead(A0);

  float voltage = (5.0 / 1023.0) * leituraResistor;

  Serial.println(voltage);

if( voltage < 3.20)
{ 
  digitalWrite(ledPin,HIGH);
}

}