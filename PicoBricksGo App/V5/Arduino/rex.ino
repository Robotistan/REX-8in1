// Kütüphaneler
#include <BLEDevice.h>      // BLE cihaz tanımı için
#include <BLEServer.h>      // BLE sunucusu oluşturmak için
#include <BLEUtils.h>       // BLE yardımcı fonksiyonlar
#include <BLE2902.h>        // BLE özellikleri için ek tanımlayıcılar
#include <ESP32Servo.h>     // ESP32 için servo motor kontrol kütüphanesi

// BLE servis ve karakteristik tanımları (UUID'ler)
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"   // Özel BLE servisi
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   // Özel BLE karakteristiği

BLECharacteristic *pCharacteristic;  // Karakteristik objesinin işaretçisi
String characteristicValue = "";     // BLE üzerinden iletilen veri saklanır

// Motor pin tanımlamaları
#define MotorPWM 13 // PWM sinyali için pin

#define MotorA1 15  // Motor A ileri
#define MotorA2 23  // Motor A geri

#define MotorB1 32  // Motor B ileri
#define MotorB2 33  // Motor B geri

#define MotorC1 27  // Motor C ileri
#define MotorC2 14  // Motor C geri

#define MotorD1 5   // Motor D ileri
#define MotorD2 4   // Motor D geri

#define horn 25     // Buzzer pini

// Hareket yönleri sabitleri
#define STOP 0       // Dur
#define FWD 1        // İleri
#define BWD 2        // Geri
#define RIGHT 3      // Sağa dön
#define LEFT 4       // Sola dön
#define FWD_RIGHT 5  // Çapraz sağ ileri
#define FWD_LEFT 6   // Çapraz sol ileri
#define BWD_RIGHT 7  // Çapraz sağ geri
#define BWD_LEFT 8   // Çapraz sol geri

// Kontrol modları
#define joystick 1   // Joystick ile kontrol
#define buttons 2    // Butonlarla kontrol

// Müzikal nota frekans tanımlamaları
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

#define NOTE_DURATION 200 // Nota çalma süresi (ms)

// Servo motor pozisyon değişkenleri (başlangıç açıları)
int position1 = 90;
int position2 = 90;
int position3 = 90;
int position4 = 90;

// Servo motor nesneleri
Servo Servo1;  // Servo 1: ileri-geri
Servo Servo2;  // Servo 2: sağ-sol
Servo Servo3;  // Servo 3: yukarı-aşağı
Servo Servo4;  // Servo 4: aç-kapa

// Genel değişkenler
int buffer[5];         // BLE'den gelen veri için tampon dizi
int control = 0;       // Kontrol modu
int i = 0;             // Sayaç
int ySpeed = 0;        // Joystick Y eksenine göre hız
int xSpeed = 0;        // Joystick X eksenine göre hız
int deadZone = 20;     // Joystick merkezinde tepkisizlik alanı
int centerX = 127;     // Joystick X ekseni merkez değeri
int centerY = 127;     // Joystick Y ekseni merkez değeri
int xValue = 0;        // X ekseni değeri
int yValue = 0;        // Y ekseni değeri

// Hareket fonksiyonu - yön ve hız parametrelerine göre motorları kontrol eder
void move(int direction, int speed) {
    speed = constrain(speed, 150, 255);  // Hızı 150-255 aralığında sınırla
    analogWrite(MotorPWM, speed);       // PWM sinyali ile motor hızını ayarla

  if (direction == FWD){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
  }
  else if (direction == BWD){
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
  }
  else if (direction == LEFT){
    digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
  }
  else if (direction == RIGHT){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
  }
  else if (direction == FWD_RIGHT){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);  
    digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
  }
  else if (direction == FWD_LEFT){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);  
  }
  else if (direction == BWD_RIGHT){
    digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);  
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
  }
  else if (direction == BWD_LEFT){
    digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH); 
  }
  else{ //STOP
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, LOW);
  }
}

// omni_move fonksiyonu - omni botlar için yönlü hareket kontrolü
void omni_move(int direction, int speed) {
    speed = constrain(speed, 150, 255);  
    analogWrite(MotorPWM, speed);

  if (direction == FWD){
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
  }
  else if (direction == BWD){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
  }
  else if (direction == LEFT){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
  }
  else if (direction == RIGHT){
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
  }
  else if (direction == FWD_RIGHT){
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, LOW);  
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
  }
  else if (direction == FWD_LEFT){
    digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
    digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, LOW);  
  }
  else if (direction == BWD_RIGHT){
    digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);  
    digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, LOW);
  }
  else if (direction == BWD_LEFT){
    digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW); 
  }
  else{ //STOP
    digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, LOW);
  }
}

// rex_horn - buzzer'ı kısa süreli ötürür
void rex_horn() {
    for(int i=0; i<50; i++){
        digitalWrite(horn, HIGH);
        delay(2);
        digitalWrite(horn, LOW);
        delay(2);
    }
}

// BLE bağlantı durumu geri çağırma fonksiyonu
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Client connected");  // Bağlantı kurulduğunda konsola yaz
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Client disconnected");  // Bağlantı kesildiğinde konsola yaz
      pServer->startAdvertising();  // BLE yeniden reklam yayınına başlasın
    }
};

// BLE karakteristik veri işleme geri çağırma sınıfı
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
          if(buffer[1] == 1){ //Wibot & Roverbot
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
                move(STOP, 0);
              }
              if(buffer[2] == 3) {  //Forward movement
                move(FWD, 255);
              }
              if(buffer[2] == 4) {  //Forward-right diagonal
                move(FWD_RIGHT, 150);
              }
              if(buffer[2] == 5) {  //Right turn
                move(RIGHT, 255);
              }
              if(buffer[2] == 6) {  //Backward-right diagonal
                move(BWD_RIGHT, 150);
              }
              if(buffer[2] == 7) {  //Backward movement
                move(BWD, 255);
              }
              if(buffer[2] == 8) {  //Backward-left diagonal
                move(BWD_LEFT, 150);
              }
              if(buffer[2] == 9) {  //Left turn
                move(LEFT, 255);
              }
              if(buffer[2] == 10) {  //Forward-left diagonal
                move(FWD_LEFT, 150);
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
                omni_move(STOP, 0);
              }
              if(buffer[2] == 3) {  //Forward movement
                omni_move(FWD, 255);
              }
              if(buffer[2] == 4) {  //Forward-right diagonal
                omni_move(FWD_RIGHT, 255);
              }
              if(buffer[2] == 5) {  //Right turn
                omni_move(RIGHT, 255);
              }
              if(buffer[2] == 6) {  //Backward-right diagonal
                omni_move(BWD_RIGHT, 255);
              }
              if(buffer[2] == 7) {  //Backward movement
                omni_move(BWD, 255);
              }
              if(buffer[2] == 8) {  //Backward-left diagonal
                omni_move(BWD_LEFT, 255);
              }
              if(buffer[2] == 9) {  //Left turn
                omni_move(LEFT, 255);
              }
              if(buffer[2] == 10) {  //Forward-left diagonal
                omni_move(FWD_LEFT, 255);
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
  Serial.begin(115200);  // Seri haberleşmeyi başlat

  // BLE başlat
  BLEDevice::init("REX 8in1");  // Cihaz adını belirle
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // BLE servisi oluştur
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // BLE karakteristiği oluştur
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue(characteristicValue);
  pService->start();               // Servisi başlat
  pServer->getAdvertising()->start();  // Yayını başlat

  characteristicValue = "";
  pCharacteristic->setValue(characteristicValue);

    // Initialize servo motors and set their default positions
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Servo1.setPeriodHertz(50);
  Servo2.setPeriodHertz(50);
  Servo3.setPeriodHertz(50);
  Servo4.setPeriodHertz(50);

  Servo1.attach(2, 600, 2500);   // Attach Servo 1 to pin 2
  Servo2.attach(26, 600, 2500);  // Attach Servo 2 to pin 26
  Servo3.attach(18, 600, 2500);  // Attach Servo 3 to pin 18
  Servo4.attach(19, 600, 2500);  // Attach Servo 4 to pin 19

  // Set initial positions
  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);

  // Initialize motor control pins
  pinMode(MotorPWM, OUTPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);

  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  pinMode(horn, OUTPUT);
}

void loop() {
  // Bu projede sürekli veri alınmadığı için loop boş bırakılmış
}
