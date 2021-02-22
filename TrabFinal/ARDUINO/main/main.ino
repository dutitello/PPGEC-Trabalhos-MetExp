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
long tlendo, texc;
float freq;

// Variáveis do acelerometro
int MPU = 0x68;
int16_t AcX, AcY, AcZ;
long t0, t=0;
char line[30];

// Variaveis do excitador
Servo servo;
int theta = 30;
int pzero = 90;
int angulo = 0;
int incr = 1;


// Chamada do Acelerometro
void fAcelerometro(){
  Serial.println("Thread acelerometro");
  // Faz leituras por tlendo segundos
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // 2 bytes for each parameters

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  t = millis() - t0;

  sprintf(line, "%9ld %6d %6d %6d", t, AcX, AcY, AcZ);
  Serial.println(line);
  Wire.endTransmission(true);
}

// Chamada do Excitador
void fExcitador(){
  Serial.println("Thread excitador");
  angulo += incr;
  if(angulo >= pzero+theta && incr > 0){
    incr = -1;
  }
  if(angulo < pzero-theta && incr < 0){
    incr = +1;
  }
  servo.write(angulo);
}


void setup(){
  // Configura thread do acelerometro
  acel.onRun(fAcelerometro);
  acel.setInterval(1); // intervalo em ms  

  // Configura thread do excitador
  exct.onRun(fExcitador);
  exct.setInterval(1); // apenas configuração inicial

  // Adiciona as threads ao controle
  ctrl.add(&acel);
  ctrl.add(&exct);

  // Inicia com threads desativadas 
  acel.enabled = false;
  exct.enabled = false;

  // Inicializa e coloca servo na posição zero (90graus)
  servo.write(pzero);

  // Inicializa serial
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  }

  // Configura acelerometro
  Wire.begin();
  Wire.beginTransmission(MPU);  // inicializa acelerometro
  Wire.write(0x6B);             // reseta configurações
  Wire.write(0x00);
  Wire.endTransmission(true); // finaliza transmissão

  // Segura 1s e marca tempo inicial
  delay(1000);
  t0 = millis();
}

void loop(){
  // Ativa o controle, mas threads estão desativadas
  ctrl.run();

  // Estabelece comunicação serial
  if (Serial.available()){
    // Recebe comandos no formato: tlendo;texc;freq*100;
    String NS;
    NS = Serial.readStringUntil(';');
    tlendo = NS.toInt();

    NS = Serial.readStringUntil(';');
    texc = NS.toInt();

    NS = Serial.readStringUntil(';');
    freq = NS.toInt()/100;
    
    t = 0;
    sprintf(line, "Iniciando processo %d %d %d", tlendo, texc, t);
    Serial.println(line);

    // Configura delay que gera velocidade no servo
    exct.setInterval(1000/4/theta/freq);

    // Ativa threads
    acel.enabled = true;
    exct.enabled = true;
  }

  t = millis() - t0;
  
  if (acel.enabled && t > tlendo){
    Serial.println("desliga acelerometro");
    acel.enabled = false;
  }

  if (exct.enabled && t > texc){
    Serial.println("desliga excitador");
    exct.enabled = false;
  }

}
