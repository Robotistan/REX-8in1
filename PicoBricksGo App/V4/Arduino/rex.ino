// Gerekli kütüphaneler dahil ediliyor
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h>

// BLE hizmeti ve karakteristikleri için benzersiz UUID’ler tanımlanıyor
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // BLE servisi için UUID
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // BLE karakteristiği için UUID

BLECharacteristic *pCharacteristic;  // Karakteristik nesnesi için işaretçi
String characteristicValue = "";     // Karakteristiğe atanacak değer

// Motor pinleri tanımlanıyor (ileri/geri kontrol)
#define MotorA1 15
#define MotorA2 23
#define MotorB1 32
#define MotorB2 33
#define MotorC1 5
#define MotorC2 4
#define MotorD1 27
#define MotorD2 14

#define horn 25 // Buzzer pini

// Hareket yönlerini temsil eden sabitler
#define STOP 0
#define FWD 1
#define BWD 2
#define RIGHT 3
#define LEFT 4
#define FWD_RIGHT 5
#define FWD_LEFT  6
#define BWD_RIGHT 7
#define BWD_LEFT  8

// Kontrol modları
#define joystick 1
#define buttons 2

// Nota frekansları (müzik için kullanılabilir)
#define NOTE_C  262
#define NOTE_CS 278
#define NOTE_D  294
#define NOTE_DS 312
#define NOTE_E  330
#define NOTE_F  350
#define NOTE_FS 370
#define NOTE_G  392
#define NOTE_GS 416
#define NOTE_A  440
#define NOTE_AS 467
#define NOTE_B  494
#define NOTE_C2 524
#define NOTE_DURATION 100 // Nota çalma süresi (ms)

// Servo motor pozisyonları
int position1 = 90;
int position2 = 90;
int position3 = 90;
int position4 = 90;

// Servo motor nesneleri
Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

// Değişken tanımlamaları
int buffer[5];          
int control = 0;        
int i = 0;              
int duty = 0;           
int ySpeed  = 0;        
int xSpeed  = 0;        
int deadZone = 20;      
int centerX = 127;      
int centerY = 127;      
int xValue = 0;         
int yValue = 0;         

// Belirtilen yönde ve hızda robotu hareket ettiren fonksiyon
void move(int direction, int speed){
  duty = constrain(speed, 150, 255); 

  if (direction == FWD){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == BWD){
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, duty);
  }
  else if (direction == LEFT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, duty);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == RIGHT){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty);
  }
  else if (direction == FWD_RIGHT){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);  
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty);
  }
  else if (direction == FWD_LEFT){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, duty);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);  
  }
  else if (direction == BWD_RIGHT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, duty);  
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == BWD_LEFT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty); 
  }
  else{ //STOP
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, LOW);
  }
}

// omni_move() fonksiyonu omni tekerlekli botlar için aynı mantıkla çalışır
void omni_move(int direction, int speed){
  duty = constrain(speed, 150, 255); 


  if (direction == FWD){
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, duty);
  }
  else if (direction == BWD){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == LEFT){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, duty);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == RIGHT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty);
  }
  else if (direction == FWD_RIGHT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, LOW);  
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty);
  }
  else if (direction == FWD_LEFT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, LOW);  
  }
  else if (direction == BWD_RIGHT){
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);  
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, LOW);
  }
  else if (direction == BWD_LEFT){
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW); 
  }
  else{ //STOP
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, LOW);
  }
}
// Kısa bir sesli uyarı verir
void rex_horn() {
    for(int i=0; i<50; i++){
        digitalWrite(horn, HIGH);
        delay(2);
        digitalWrite(horn, LOW);
        delay(2);
    }
}

// BLE bağlantı olayları için geri çağırma sınıfı
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Client connected");
    }
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Client disconnected");
      pServer->startAdvertising();  // Bağlantı kopunca yeniden reklam yayınla
    }
};

// BLE karakteristik verisi yazıldığında çağrılan sınıf
class MyCallbacks: public BLECharacteristicCallbacks {
      void onWrite(BLECharacteristic *pCharacteristic) {
        const uint8_t* value = pCharacteristic->getData();  // Get the value written to the characteristic
        size_t length = pCharacteristic->getLength(); 

      // Process the value if it has been received
      if (length > 0) {
        for (i = 0; i < length; i++){
          buffer[i] = value[i];
          //Serial.println(buffer[i]);
          //delay(100);
        }

        if(buffer[0] == 75){  // Header byte for Rex commands
          if(buffer[1] == 1){ // Wibot & Roverbot
            if (buffer[2] == 1){
              control = joystick;
            }
            else{
              control = buttons;
            }

            if(buffer[2] == 2){  //Horn
              rex_horn();
            }

            if(control == buttons){ //Directional Buttons
              if(buffer[2] == 99) {  //Stop
                move(STOP,0);
              }
              if(buffer[2] == 3) {  //Forward movement
                move(FWD,255);
              }
              if(buffer[2] == 4) {  //Forward-right diagonal
                move(FWD_RIGHT,150);
              }
              if(buffer[2] == 5) {  //Right turn
                move(RIGHT,255);
              }
              if(buffer[2] == 6) {  //Backward-right diagonal
                move(BWD_RIGHT,150);
              }
              if(buffer[2] == 7) {  //Backward movement
                move(BWD,255);
              }
              if(buffer[2] == 8) {  //Backward-left diagonal
                move(BWD_LEFT,150);
              }
              if(buffer[2] == 9) {  //Left turn
                move(LEFT,255);
              }
              if(buffer[2] == 10) {  //Forward-left diagonal
                move(FWD_LEFT,150);
              }
            }
            else{ //Joystick
              xValue = buffer[3]; //Joystick X-axis
              yValue = buffer[4]; //Joystick Y-axis

              ySpeed = map(abs(yValue - centerY), 0, 127, 0, 255);  //Map Y-axis for forward/backward speed
              xSpeed = map(abs(xValue - centerX), 0, 127, 0, 255);  //Map X-axis for turning speed

              if (xValue == 0 && yValue == 0) {   //Check if joystick is close to the center for stopping
                move(STOP,0);
              }
              else if (yValue < centerY - deadZone && abs(xValue - centerX) <= deadZone) {   //Forward movement
                move(FWD, ySpeed);
              }
              else if (yValue > centerY + deadZone && abs(xValue - centerX) <= deadZone) {   //Backward movement
                move(BWD, ySpeed);
              }
              else if (xValue < centerX - deadZone && abs(yValue - centerY) <= deadZone) {   //Left turn
                move(LEFT, xSpeed);
              }
              else if (xValue > centerX + deadZone && abs(yValue - centerY) <= deadZone) {   //Right turn
                move(RIGHT, xSpeed);
              }
              else if (yValue < centerY - deadZone && xValue > centerX + deadZone) {  //Forward-right diagonal
                move(FWD_RIGHT, ySpeed);
              }
              else if (yValue < centerY - deadZone && xValue < centerX - deadZone) {  //Forward-left diagonal
                move(FWD_LEFT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue > centerX + deadZone) {  //Backward-right diagonal
                move(BWD_RIGHT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue < centerX - deadZone) {  //Backward-left diagonal
                move(BWD_LEFT, ySpeed);
              }
              else{
                move(STOP,0);
              }
            }
          }
          else if(buffer[1] == 2){ //Omnibot
            if (buffer[2] == 1){
              control = joystick;
            }
            else{
              control = buttons;
            }

            if(buffer[2] == 2){  //Horn
              rex_horn();
            }

            if(control == buttons){ //Directional Buttons
              if(buffer[2] == 99) {  //Stop
                omni_move(STOP,0);
              }
              if(buffer[2] == 3) {  //Forward movement
                omni_move(FWD,255);
              }
              if(buffer[2] == 4) {  //Forward-right diagonal
                omni_move(FWD_RIGHT,255);
              }
              if(buffer[2] == 5) {  //Right turn
                omni_move(RIGHT,255);
              }
              if(buffer[2] == 6) {  //Backward-right diagonal
                omni_move(BWD_RIGHT,255);
              }
              if(buffer[2] == 7) {  //Backward movement
                omni_move(BWD,255);
              }
              if(buffer[2] == 8) {  //Backward-left diagonal
                omni_move(BWD_LEFT,255);
              }
              if(buffer[2] == 9) {  //Left turn
                omni_move(LEFT,255);
              }
              if(buffer[2] == 10) {  //Forward-left diagonal
                omni_move(FWD_LEFT,255);
              }
            }
            else{ //Joystick
              xValue = buffer[3]; //Joystick X-axis
              yValue = buffer[4]; //Joystick Y-axis

              ySpeed = map(abs(yValue - centerY), 0, 127, 0, 255);  //Map Y-axis for forward/backward speed
              xSpeed = map(abs(xValue - centerX), 0, 127, 0, 255);  //Map X-axis for turning speed

              if (xValue == 0 && yValue == 0) {   //Check if joystick is close to the center for stopping
                omni_move(STOP,0);
              }
              else if (yValue < centerY - deadZone && abs(xValue - centerX) <= deadZone) {   //Forward movement
                omni_move(FWD, ySpeed);
              }
              else if (yValue > centerY + deadZone && abs(xValue - centerX) <= deadZone) {   //Backward movement
                omni_move(BWD, ySpeed);
              }
              else if (xValue < centerX - deadZone && abs(yValue - centerY) <= deadZone) {   //Left turn
                omni_move(LEFT, xSpeed);
              }
              else if (xValue > centerX + deadZone && abs(yValue - centerY) <= deadZone) {   //Right turn
                omni_move(RIGHT, xSpeed);
              }
              else if (yValue < centerY - deadZone && xValue > centerX + deadZone) {  //Forward-right diagonal
                omni_move(FWD_RIGHT, ySpeed);
              }
              else if (yValue < centerY - deadZone && xValue < centerX - deadZone) {  //Forward-left diagonal
                omni_move(FWD_LEFT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue > centerX + deadZone) {  //Backward-right diagonal
                omni_move(BWD_RIGHT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue < centerX - deadZone) {  //Backward-left diagonal
                omni_move(BWD_LEFT, ySpeed);
              }
              else{
                omni_move(STOP,0);
              }
            }
          }
          else if(buffer[1] == 3){ //Armbot
            if(buffer[2] == 1){  //Servo Motors Reset
              position1 = 90;
              position2 = 90;
              position3 = 90;
              position4 = 90;

              Servo1.write(position1);
              Servo2.write(position2);
              Servo3.write(position3);
              Servo4.write(position4);
            }
            else if(buffer[2] == 2){  //Horn
              rex_horn();
            }
            else if(buffer[2] == 3){ //DC Motors Control
              xValue = buffer[3]; // Joystick X-axis
              yValue = buffer[4]; // Joystick Y-axis

              ySpeed = map(abs(yValue - centerY), 0, 127, 0, 255);  // Map Y-axis for forward/backward speed
              xSpeed = map(abs(xValue - centerX), 0, 127, 0, 255);  // Map X-axis for turning speed

              if (xValue == 0 && yValue == 0) {   // Check if joystick is close to the center for stopping
                move(STOP,0);
              }
              else if (yValue < centerY - deadZone && abs(xValue - centerX) <= deadZone) {   // Forward movement
                move(FWD, ySpeed);
              }
              else if (yValue > centerY + deadZone && abs(xValue - centerX) <= deadZone) {   // Backward movement
                move(BWD, ySpeed);
              }
              else if (xValue < centerX - deadZone && abs(yValue - centerY) <= deadZone) {   // Left turn
                move(LEFT, xSpeed);
              }
              else if (xValue > centerX + deadZone && abs(yValue - centerY) <= deadZone) {   // Right turn
                move(RIGHT, xSpeed);
              }
              else if (yValue < centerY - deadZone && xValue > centerX + deadZone) {  // Forward-right diagonal
                move(FWD_RIGHT, ySpeed);
              }
              else if (yValue < centerY - deadZone && xValue < centerX - deadZone) {  // Forward-left diagonal
                move(FWD_LEFT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue > centerX + deadZone) {  // Backward-right diagonal
                move(BWD_RIGHT, ySpeed);
              }
              else if (yValue > centerY + deadZone && xValue < centerX - deadZone) {  // Backward-left diagonal
                move(BWD_LEFT, ySpeed);
              }
              else{
                move(STOP,0);
              }
            }
            else if(buffer[2] == 4){  //Servo Motors Control
              if(buffer[3] == 1){ //Servo 1 Left
                if (position1 > 40) {
                  position1 = position1 - 5;
                  Servo1.write(position1);
                }
              }
              if(buffer[3] == 2){   //Servo 1 Right
                if (position1 < 140) {
                  position1 = position1 + 5;
                  Servo1.write(position1);
                }
              }
              if(buffer[3] == 3){   //Servo 2 Up
                if (position2 < 140) {
                  position2 = position2 + 5;
                  Servo2.write(position2);
                }
              }
              if(buffer[3] == 4){   //Servo 2 Down
                if (position2 > 0) {
                  position2 = position2 - 5;
                  Servo2.write(position2);
                }
              }
              if(buffer[3] == 5){   //Servo 3 Down
                if (position3 > 30) {
                  position3 = position3 - 5;
                  Servo3.write(position3);
                }
              }
              if(buffer[3] == 6){   //Servo 3 Up
                if (position3 < 150) {
                  position3 = position3 + 5;
                  Servo3.write(position3);
                }
              }
              if(buffer[3] == 7){   //Servo 4 Open
                if (position4 > 90) {
                  position4 = position4 - 5;
                  Servo4.write(position4);
                }
              }
              if(buffer[3] == 8){   //Servo 4 Close
                if (position4 < 160) {
                  position4 = position4 + 5;
                  Servo4.write(position4);
                }
              }
            }
          }
          else if(buffer[1] == 5){ //Piano
            if(buffer[2] == 1){ //C1
              tone(horn, NOTE_C, NOTE_DURATION);
            }
            else if(buffer[2] == 2){ //D
              tone(horn, NOTE_D, NOTE_DURATION);
            }
            else if(buffer[2] == 3){ //E
              tone(horn, NOTE_E, NOTE_DURATION);
            }
            else if(buffer[2] == 4){ //F
              tone(horn, NOTE_F, NOTE_DURATION);
            }
            else if(buffer[2] == 5){ //G
              tone(horn, NOTE_G, NOTE_DURATION);
            }
            else if(buffer[2] == 6){ //A
              tone(horn, NOTE_A, NOTE_DURATION);
            }
            else if(buffer[2] == 7){ //B
              tone(horn, NOTE_B, NOTE_DURATION);
            }
            else if(buffer[2] == 8){ //C2
              tone(horn, NOTE_C2, NOTE_DURATION);
            }
            else if(buffer[2] == 9){ //CS1
              tone(horn, NOTE_CS, NOTE_DURATION);
            }
            else if(buffer[2] == 10){ //DS1
              tone(horn, NOTE_DS, NOTE_DURATION);
            }
            else if(buffer[2] == 11){ //FS1
              tone(horn, NOTE_FS, NOTE_DURATION);
            }
            else if(buffer[2] == 12){ //GS1
              tone(horn, NOTE_GS, NOTE_DURATION);
            }
            else if(buffer[2] == 13){ //AS1
              tone(horn, NOTE_AS, NOTE_DURATION);
            }
            delay(5);
            noTone(horn);
          }
          else{
            Serial.println("Wrong data");
          }
      }
      //Clear Buffer
      for(i=0; i<5; i++)
        buffer[i] = 0;
      }
    }
};

void setup() {
  Serial.begin(115200);

  // BLE başlatılır ve sunucu + servis + karakteristik oluşturulur
  BLEDevice::init("REX 8in1");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue(characteristicValue);
  pService->start();
  pServer->getAdvertising()->start();

  // Servo motorlar ayarlanır ve başlangıç pozisyonlarına getirilir
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Servo1.setPeriodHertz(50);
  Servo2.setPeriodHertz(50);
  Servo3.setPeriodHertz(50);
  Servo4.setPeriodHertz(50);

  Servo1.attach(2, 600, 2500);
  Servo2.attach(26, 600, 2500);
  Servo3.attach(18, 600, 2500);
  Servo4.attach(19, 600, 2500);

  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);

  // Motor pinleri çıkış olarak ayarlanır
  pinMode(MotorA1, OUTPUT); pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT); pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT); pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT); pinMode(MotorD2, OUTPUT);

  pinMode(horn, OUTPUT);
}

void loop() {
  // loop boş bırakılmış çünkü tüm kontrol BLE üzerinden geri çağırma fonksiyonları ile yapılmakta
}
