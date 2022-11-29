
/*
     CÓDIGO:  Q0000
     AUTOR:   BrincandoComIdeias
     LINK:    https://www.youtube.com/brincandocomideias
     APRENDA: https://cursodearduino.net/ ; https://cursoderobotica.net
     COMPRE:  https://www.arducore.com.br/
     SKETCH:  Elevador Arducore
     DATA:    19/11/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// PINOS REED (MAGNÉTICOS)
#define pinT 9
#define pin1 10
#define pin2 11
#define pin3 12
// BOTÕES

#define pinBT 3 // BOTÃO TERREO
#define pinB1 4 // BOTÃO ANDAR 1
#define pinB2 5 // BOTÃO ANDAR 2
#define pinB3 6 // BOTÃO ANDAR 3

#define pinSensorFogo 2

#define pinServo 7

// DEFINE VELOCIDADE DO SERVO PARA AS FUNCÕES DE CIMA, BAIXO, FOGO E PARADO.
#define VelUP 65
#define VelDW 115
#define VelFogo 100
#define Parado 90

// INSTANCIANDO OBJETOS

Servo servo;

// DECLARAÇÃO DE FUNÇÕES
void moverCabine(byte andar, byte velMaxUP = VelUP, byte velMaxDown = VelDW);
byte verificaAndar();

//void comandoSerial();
//void lerSensores();

// DECLARAÇÃO DE VARIÁVEIS
byte sensorAndar[] = {pinT, pin1, pin2, pin3};
int  delayAndar[] = {250, 300, 350, 400};
byte andarAtual;
byte botaoApertado;
bool fogo = false;

// INCLUSÃO DO LCD
#include "complemento.h"

void setup() {
  Serial.begin(9600);

  servo.attach(pinServo);

  pinMode(pinT, INPUT);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);

  attachInterrupt(digitalPinToInterrupt(pinSensorFogo), incendioDetectado, FALLING);

  configuraDisplay();
  andarAtual = verificaAndar(); // IDENTIFICA SE ESTÁ PARADO EM ALGUM ANDAR
  exibeDisplay(PARADO, andarAtual);

  Serial.println("inicializacao finalizada");
}

void loop() {
 
 // SE FOR IDENTIFICADO FOGO EXECUTA O BLOCO ABAIXO
  if (fogo) {
    exibeDisplay(FOGO, 0); // AVISA QUE FOI IDENTIFICADO FOGO
    
    if (digitalRead(sensorAndar[0])) {
      servo.write(VelFogo); // DESCE LENTAMENTE

      while (digitalRead(sensorAndar[0])) { // LOOP PARA VERIFICAR SE CHEGOU NO TÉRREO

      }
      delay(400); // PEQUENO DELAY PARA ACERTAR A POSIÇÃO
      servo.write(Parado); //PARA O SERVO
    }
    
    lcd.setCursor(0, 1);
    lcd.print("TRAVADO "); // AVISA NO DISPLAY QUE ESTÁ TRAVADO

    while (1) {} // APÓS CHEGAR AO TERREO ENTRA NO LOOP INFINITO

  }

  // VERIFICA QUAL BOTÃO FOI PRESSIONADO E MOVE A CABINE ATÉ O ANDAR CHAMADO
  if (digitalRead(pinBT) == HIGH) {
    moverCabine(0);
  }
  else if (digitalRead(pinB1) == HIGH) {
    moverCabine(1);
  }
  else if (digitalRead(pinB2) == HIGH) {
    moverCabine(2);
  }
  else if (digitalRead(pinB3) == HIGH) {
    moverCabine(3);
  }

} // FIM DO LOOP


// IMPLEMENTO DE FUNÇÕES
void moverCabine(byte andar, byte velMaxUP = VelUP, byte velMaxDown = VelDW) {

  if (andarAtual < andar) { // VERIFICA SE O ANDAR ATUAL É MENOR QUE O ANDAR QUE CHAMOU
    servo.write(velMaxUP);
    exibeDisplay(SUBINDO, andar);

    while (digitalRead(sensorAndar[andar]) && !fogo) { // LOOP ENQUANTO NÃO CHEGA NO ANDAR E NÃO TEM SINAL DE FOGO

    }

    // SE HOUVER FOGO EXECUTA O BLOCO ABAIXO
    if (fogo) {
      servo.write(Parado); // PARA O SERVO
      return; // SAI DA FUNÇÃO SEM EXECUTAR MAIS NADA
    }

    // SE NÃO HOUVER FOGO CONTINUA EXECUTANDO O BLOCO ABAIXO
    delay(delayAndar[andar]);
    servo.write(Parado);
    andarAtual = andar;
    exibeDisplay(PARADO, andarAtual);

  } else if (andarAtual > andar) { // VERIFICA SE O ANDAR ATUAL É MAIOR QUE O ANDAR QUE CHAMOU
    servo.write(velMaxDown);
    exibeDisplay(DESCENDO, andar);

    while (digitalRead(sensorAndar[andar]) && !fogo) {// LOOP ENQUANTO NÃO CHEGA NO ANDAR E NÃO TEM SINAL DE FOGO

    }

    // SE HOUVER FOGO EXECUTA O BLOCO ABAIXO
    if (fogo) {
      servo.write(Parado);
      return;// SAI DA FUNÇÃO SEM EXECUTAR MAIS NADA
    }

    // SE NÃO HOUVER FOGO CONTINUA EXECUTANDO O BLOCO ABAIXO
    delay(delayAndar[andar]);

    servo.write(Parado);
    andarAtual = andar;
    exibeDisplay(PARADO, andarAtual);

  }

}

byte verificaAndar() {
  if (!digitalRead(sensorAndar[0])) {
    delay(delayAndar[0]);
    servo.write(Parado);
    return 0;
  }
  else if (!digitalRead(sensorAndar[1])) {
    delay(delayAndar[1]);
    servo.write(Parado);
    return 1;
  }
  else if (!digitalRead(sensorAndar[2])) {
    delay(delayAndar[2]);
    servo.write(Parado);
    return 2;
  }
  else if (!digitalRead(sensorAndar[3])) {
    delay(delayAndar[3]);
    servo.write(Parado);
    return 3;
  }
  else {
    servo.write(VelDW);
    delay(10);
    verificaAndar();
  }
}

// FUNÇÃO DA INTERRUPÇÃO
void incendioDetectado() {
  fogo = true;
  //  Serial.println("fogo");
}

//void lerSensores() {
//  for (byte i = 0; i < 4; i++) {
//    Serial.print("Sensor 0"); Serial.print(i); Serial.print(": "); Serial.println(digitalRead(sensorAndar[i]));
//  }
//  Serial.println("--------------------------------------");
//  delay(250);
//}
//
//void comandoSerial() {
//  if (Serial.available()) {
//    char c = Serial.read();
//    moverCabine(c - '0');
//    lerSensores();
//  }
//}
