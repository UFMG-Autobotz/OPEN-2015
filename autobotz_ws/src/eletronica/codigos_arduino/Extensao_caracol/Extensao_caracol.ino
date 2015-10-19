int potPin = A0;    
int motorVelPin = 9;
int motor2DirPin = 8;
int motor2VelPin = 10;
int motorDirPin = 11;
int botaoPin = 12;
int direcao;
int time;

void setup() {
  pinMode(motorVelPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motor2VelPin, OUTPUT);
  pinMode(motor2DirPin, OUTPUT);
  pinMode(botaoPin, INPUT);
  
  direcao = HIGH;
  time=0;
  
  Serial.begin(9600);
}

void loop() {
  
  int potValue;
  int velocidade;
  
  int botao;
  
  
  
  botao = digitalRead(botaoPin);
  

  if (botao==HIGH && (millis()-time) > 1000){
    if (direcao == HIGH)
      direcao = LOW;

    else
      direcao = HIGH; 

    time = millis();
  }
  potValue = analogRead(potPin);    

  digitalWrite(motorDirPin, direcao);
  digitalWrite(motor2DirPin, direcao);
  
  velocidade = (potValue/4);
  
  if (direcao == LOW)
    velocidade = 255 - velocidade;
  
  analogWrite(motorVelPin, velocidade);
 
  velocidade +=20;
  if (velocidade >255)
    velocidade = 255;
   analogWrite(motor2VelPin, velocidade);
  

  Serial.print(velocidade);
   Serial.print(", ");
  Serial.println(direcao);
  
  delay(100);
                
}
