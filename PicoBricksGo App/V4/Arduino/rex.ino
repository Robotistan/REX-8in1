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
  duty = constrain(speed, 150, 255);  // Hızı sınırla

  // Yöne göre motorlara sinyal gönder
  if (direction == FWD){ // İleri
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == BWD){ // Geri
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, duty);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, duty);
  }
  else if (direction == LEFT){ // Sola dönüş
    analogWrite(MotorA1, LOW);  analogWrite(MotorA2, duty);
    analogWrite(MotorB1, LOW);  analogWrite(MotorB2, duty);
    analogWrite(MotorC1, duty); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, duty); analogWrite(MotorD2, LOW);
  }
  else if (direction == RIGHT){ // Sağa dönüş
    analogWrite(MotorA1, duty); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, duty); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW);  analogWrite(MotorC2, duty);
    analogWrite(MotorD1, LOW);  analogWrite(MotorD2, duty);
  }
  // Diagonal ve diğer yönler için motor kombinasyonları tanımlanır
  else if (direction == FWD_RIGHT){ ... }
  else if (direction == FWD_LEFT){ ... }
  else if (direction == BWD_RIGHT){ ... }
  else if (direction == BWD_LEFT){ ... }
  else { // STOP
    analogWrite(MotorA1, LOW); analogWrite(MotorA2, LOW);
    analogWrite(MotorB1, LOW); analogWrite(MotorB2, LOW);
    analogWrite(MotorC1, LOW); analogWrite(MotorC2, LOW);
    analogWrite(MotorD1, LOW); analogWrite(MotorD2, LOW);
  }
}

// omni_move() fonksiyonu omni tekerlekli botlar için aynı mantıkla çalışır

// Kısa bir sesli uyarı verir
void rex_horn() {
  tone(horn, 262, NOTE_DURATION);
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
        const uint8_t* value = pCharacteristic->getData();
        size_t length = pCharacteristic->getLength();

        if (length > 0) {
            for (i = 0; i < length; i++) buffer[i] = value[i];

            if(buffer[0] == 75){  // Komut başlangıcı (örneğin 0x4B = 'K')

                // buffer[1] farklı bot türlerini temsil eder
                if(buffer[1] == 1){ /* WiBot veya Rover */ }
                else if(buffer[1] == 2){ /* OmniBot */ }
                else if(buffer[1] == 3){ /* ArmBot */ }
                else if(buffer[1] == 5){ /* Piano modu */ }
                else {
                    Serial.println("Wrong data");
                }
            }

            // Buffer'ı sıfırla
            for(i=0; i<5; i++) buffer[i] = 0;
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
