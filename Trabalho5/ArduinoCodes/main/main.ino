/*
Código para excitar e medir acelerações (giro opcionalmente) usando um arduino uno, acelerometro MPU6050 e um servo 

Eduardo Pagnussat Titello - 2021
*/

#include <Wire.h>
#include <Servo.h>
// Usando o sistema de threads do IvanSeidel para facilitar controle do excitador e acelerometro (https://github.com/ivanseidel/ArduinoThread)
#include <Thread.h>
#include <ThreadController.h>

// Cria controlador e as Threads
ThreadController ctrl = ThreadController();
Thread acel = Thread();
Thread exct = Thread();

// Variaveis dos comandos
int N1, N2, N3;

// Chamada do Acelerometro
void fAcelerometro(){
  Serial.println("Acelerometro rodando");
  Serial.print("N2=");
  Serial.println(N2);
}

// Chamada do Excitador
void fExcitador(){
  Serial.println("Excitador rodando");
  Serial.print("N3=");
  Serial.println(N3);
}

void setup(){
  // Inicializa serial
  Serial.begin(9600);
  
  // Configura thread do acelerometro
  acel.onRun(fAcelerometro);
  acel.setInterval(50); // intervalo em ms  

  // Configura thread do excitador
  exct.onRun(fExcitador);
  exct.setInterval(100); // intervalo em ms

  // Adiciona as threads ao controle
  ctrl.add(&acel);
  ctrl.add(&exct);

  // Inicia com threads desativadas 
  acel.enabled = false;
  exct.enabled = false;  
}

void loop(){
  // Ativa o controle, mas threads não
  ctrl.run();

  // Recebe comandos: leituras; freq; tempo
  if (Serial.available() > 0){
    String NS;
    NS = Serial.readStringUntil(';');
    N1 = NS.toInt();

    NS = Serial.readStringUntil(';');
    N2 = NS.toInt();

    NS = Serial.readStringUntil(';');
    N3 = NS.toInt();
  }

  
  if(N1 == 1){
    acel.enabled = true;
    exct.enabled = true;
  }
  else {
    acel.enabled = false;
    exct.enabled = false;
  }
}
