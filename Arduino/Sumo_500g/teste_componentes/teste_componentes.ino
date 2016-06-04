
#include <SoftwareSerial.h>

#define pinLinhaFD A0 // pino de detector de linha FD
#define pinLinhaFE A1 // pino de detector de linha FE
#define pinLinhaT A2  // pino de detector de linha Tras

#define pinDistacia A3 // Pino de sensor infra vermelho

#define pinLedR 9 // pino do led R 
#define pinLedG 11 // pino do led G
#define pinLedB 10 // pino do led B

#define pinBluetoothTX 13 // bluetooth
#define pinBluetoothRX 12 // Pin bluetooth

#define pinIN1 2 // Pin motor + motor1
#define pinIN2 4 // Pin motor - motor1
#define pinIN3 5 // Pin motor + motor2
#define pinIN4 7 // Pin motor - motor2
#define pinENA 3 // Define a potênica do motor A
#define pinENB 6 // Define a potênica do motor B

boolean cond = true; // Condição que ativa a autonomia do robo
char carac = 'z';

// Branca = 0 ; preto = 1

SoftwareSerial serial1(pinBluetoothRX, pinBluetoothTX);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  inicie();
  inicie2();

  while (carac != 'e') {
    carac = (char) serial1.readString().charAt(0);

  }



}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:

  if (serial1.available()) {

    switch (carac) {
      case 'e':
        estrategia1();
        sensorV();
        

        break;

    }



  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void inicie(void) {
  pinMode(pinLinhaFD, INPUT);
  pinMode(pinLinhaFE, INPUT);
  pinMode(pinLinhaT, INPUT);

  pinMode(pinLedR, OUTPUT);
  pinMode(pinLedG, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  digitalWrite(pinLedR, LOW);
  digitalWrite(pinLedG, LOW);
  digitalWrite(pinLedB, LOW);

  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENA, 0);
  analogWrite(pinENB, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void inicie2() {
  serial1.begin(9600);
  serial1.setTimeout(100);
  controleLedRGB(255, 0, 0);
  delay(1000);
  controleLedRGB(0, 255, 0);
  delay(1000);
  controleLedRGB(0, 0, 255);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void controleLedRGB(int r, int g, int b) {
  analogWrite(pinLedR, 255 - r);
  analogWrite(pinLedG, 255 - g);
  analogWrite(pinLedB, 255 - b);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void leituraLinha() {
  serial1.print("LinhaFD=");
  serial1.print(digitalRead(pinLinhaFD));
  serial1.print(";");
  serial1.print("LinhaFE=");
  serial1.print(digitalRead(pinLinhaFE));
  serial1.print(";");
  serial1.print("LinhaT=");
  serial1.print(digitalRead(pinLinhaT));
  serial1.println(";");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void leituraDistancia() {
  serial1.print("Distancia=");
  serial1.print(analogRead(pinDistacia));
  serial1.print(";");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sensorV() {
  
  int estado_sensor = 0;// sensor inflavermelho

  estado_sensor = analogRead(pinDistacia);
  if (estado_sensor >= 100 ) {
    controleMotor('a', 255); //frente
    delay(200);
  }
  
}

void controleMotor(char op, int potencia) {
  switch (op) {
    case 'c': // Esquerda
      digitalWrite(pinIN1, HIGH);
      digitalWrite(pinIN2, LOW);
      digitalWrite(pinIN3, HIGH);
      digitalWrite(pinIN4, LOW);
      analogWrite(pinENA, potencia);
      analogWrite(pinENB, potencia);
      break;
    case 'b': // Direita
      digitalWrite(pinIN1, LOW);
      digitalWrite(pinIN2, HIGH);
      digitalWrite(pinIN3, LOW);
      digitalWrite(pinIN4, HIGH);
      analogWrite(pinENA, potencia);
      analogWrite(pinENB, potencia);
      break;
    case 'd': // Tráz
      digitalWrite(pinIN1, LOW);
      digitalWrite(pinIN2, HIGH);
      digitalWrite(pinIN3, HIGH);
      digitalWrite(pinIN4, LOW);
      analogWrite(pinENA, potencia);
      analogWrite(pinENB, potencia);
      break;
    case 'a': // Frente
      digitalWrite(pinIN1, HIGH);
      digitalWrite(pinIN2, LOW);
      digitalWrite(pinIN3, LOW);
      digitalWrite(pinIN4, HIGH);
      analogWrite(pinENA, potencia);
      analogWrite(pinENB, potencia);
      break;
    default: // Desligado
      digitalWrite(pinIN1, LOW);
      digitalWrite(pinIN2, LOW);
      digitalWrite(pinIN3, LOW);
      digitalWrite(pinIN4, LOW);
      analogWrite(pinENA, 0);
      analogWrite(pinENB, 0);
      break;
  }
}

void estrategia1() {
  leituraLinha();
  leituraDistancia();

  if (digitalRead(pinLinhaFD) && digitalRead(pinLinhaFD)) {
    leituraLinha();
    controleMotor('a', 255);
    controleLedRGB(255, 0, 0);
    

  } 
  if (!digitalRead(pinLinhaFD)) {

    leituraLinha();
    controleLedRGB(0, 255, 0);
    controleMotor('d', 255);
    delay(1000);
    controleMotor('c', 255);
    delay(200);
    controleMotor('a', 255);
    

  } 
  if (!digitalRead(pinLinhaFE)) {
    leituraLinha();
    controleLedRGB(0, 0, 255);
    controleMotor('d', 255);
    delay(1000);
    controleMotor('b', 255);
    delay(200);
    controleMotor('a', 255);
    


  } 
  if (!digitalRead(pinLinhaT)) {
    controleLedRGB(0, 255, 255);
    controleMotor('a', 255);

  }
}

