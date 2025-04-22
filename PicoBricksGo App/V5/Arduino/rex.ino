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

    // Yön değerine göre motor pinlerine sinyal gönder
    // (aşağıda her hareket yönü için motor pinleri set edilir)
    ...
}

// omni_move fonksiyonu - omni botlar için yönlü hareket kontrolü
void omni_move(int direction, int speed) {
    ...
}

// rex_horn - buzzer'ı kısa süreli ötürür
void rex_horn() {
  tone(horn, 262, NOTE_DURATION);  // 262 Hz frekansında kısa ses çal
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
        const uint8_t* value = pCharacteristic->getData();  // Gelen veriyi al
        size_t length = pCharacteristic->getLength();       // Veri uzunluğunu al

        // Gelen veriyi buffer dizisine kopyala
        ...
        // Verinin başına göre hangi robot modunun seçildiğini kontrol et (WiBot, OmniBot, ArmBot, Piano)
        ...
        // Her moda göre gelen buffer[2], [3], [4] değerleriyle servo, motor ya da buzzer kontrolü yapılır
        ...
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
