//"""REX 8in1 Arm Bot"""
// ESP32 kart sürümünün 2.0.11 olması gerekir

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Arduino.h>
#include <ESP32Servo.h>

// Motor tipi tanımı: DC motor mu Servo motor mu?
enum MOTOR_TYPE {
  DC_MOTOR,
  SERVO_MOTOR
};

enum MOTOR_TYPE motorType = DC_MOTOR; // Başlangıçta DC motor aktif

// DC Motor pinleri tanımlanıyor
#define MotorPWM 13 // PWM pini (hız kontrolü)

#define MotorA1 15  // İleri
#define MotorA2 23  // Geri

#define MotorB1 32  // İleri
#define MotorB2 33  // Geri

#define MotorC1 5   // İleri
#define MotorC2 4   // Geri

#define MotorD1 27  // İleri
#define MotorD2 14  // Geri

// Servo motor pozisyonlarının başlangıç değerleri
int position1 = 90;
int position2 = 90;
int position3 = 90;
int position4 = 90;

// Buzzer (korna) pini
int horn = 25;

// Servo motor nesneleri tanımlanıyor
Servo Servo1;  // İleri-Geri hareket
Servo Servo2;  // Sağ-Sol hareket
Servo Servo3;  // Yukarı-Aşağı hareket
Servo Servo4;  // Aç-Kapa (pençe)

void setup() {
  // Servo için zamanlayıcılar ayrılıyor
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Servo frekansı ayarlanıyor
  Servo1.setPeriodHertz(50);
  Servo2.setPeriodHertz(50);
  Servo3.setPeriodHertz(50);
  Servo4.setPeriodHertz(50);

  pinMode(horn, OUTPUT); // Buzzer çıkış olarak ayarlanıyor

  // Servo motorlar pinlere bağlanıyor
  Servo1.attach(2, 600, 2500);
  Servo2.attach(26, 600, 2500);
  Servo3.attach(18, 600, 2500);
  Servo4.attach(19, 600, 2500);

  // Servo motorlar başlangıç konumuna ayarlanıyor
  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);

  pinMode(MotorPWM, OUTPUT); // PWM pini çıkış olarak ayarlanıyor

  // DC motor pinleri çıkış olarak ayarlanıyor
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  Serial.begin(115200);       // Seri haberleşme başlatılıyor
  Dabble.begin("REX_ROBOT");  // Bluetooth adı ayarlanıyor
}

void loop() {
  // Bluetooth üzerinden gelen veriler okunuyor
  Dabble.processInput();

  stop(); // Her loop başında robot durduruluyor

  // SELECT tuşuna basıldığında servo motor modu aktif ediliyor
  if (GamePad.isSelectPressed()) {
    motorType = SERVO_MOTOR;
    rex_horn(); // Buzzer çal

    // Servo pozisyonları sıfırlanıyor (lokal değişken hatası var burada)
    int position1 = 90;
    int position2 = 90;
    int position3 = 90;
    int position4 = 90;
  }

  // START tuşuna basıldığında DC motor modu aktif ediliyor
  if (GamePad.isStartPressed()) {
    motorType = DC_MOTOR;
    rex_horn();
  }

  // Aktif moda göre motor fonksiyonu çağrılıyor
  switch (motorType) {
    case DC_MOTOR:
      dc_motor();
      break;
    case SERVO_MOTOR:
      servo_motor();
      break;
  }

  // Gamepad verileri (debug için kullanılabilir)
  int a = GamePad.getAngle();
  int b = GamePad.getRadius();
  float c = GamePad.getXaxisData();
  float d = GamePad.getYaxisData();
}

// Servo motorları kontrol eden fonksiyon
void servo_motor() {
  if (GamePad.isUpPressed() && position2 > 0) position2--;
  if (GamePad.isDownPressed() && position2 < 140) position2++;
  if (GamePad.isRightPressed() && position1 < 140) position1++;
  if (GamePad.isLeftPressed() && position1 > 40) position1--;
  if (GamePad.isSquarePressed() && position4 < 160) position4++;
  if (GamePad.isCirclePressed() && position4 > 90) position4--;
  if (GamePad.isCrossPressed() && position3 > 30) position3--;
  if (GamePad.isTrianglePressed() && position3 < 150) position3++;

  delay(10); // Hareketi yumuşatmak için kısa gecikme

  // Servo motorlara yeni pozisyonlar gönderiliyor
  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);
}

// DC motorları kontrol eden fonksiyon
void dc_motor() {
  if (GamePad.isUpPressed()) forward();
  if (GamePad.isDownPressed()) backward();
  if (GamePad.isLeftPressed()) left();
  if (GamePad.isRightPressed()) right();

  if (GamePad.isSquarePressed()) rex_horn();
  if (GamePad.isCirclePressed()) rex_horn();
  if (GamePad.isCrossPressed()) rex_horn();

  // Triangle tuşuna basıldığında servo motorlar sıfırlanıyor
  if (GamePad.isTrianglePressed()) {
    Servo1.write(90);
    Servo2.write(90);
    Servo3.write(90);
    Servo4.write(90);
  }
}

// İleri gitme fonksiyonu
void forward() {
  digitalWrite(MotorPWM, HIGH); // PWM aktif
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Sağa dönme fonksiyonu
void right() {
  digitalWrite(MotorPWM, HIGH);
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
}

// Sola dönme fonksiyonu
void left() {
  digitalWrite(MotorPWM, HIGH);
  digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Tüm motorları durduran fonksiyon
void stop() {
  digitalWrite(MotorPWM, LOW);
  digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, LOW);
}

// Geri gitme fonksiyonu
void backward() {
  digitalWrite(MotorPWM, HIGH);
  digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
}

// Buzzer (korna) çalma fonksiyonu
void rex_horn() {
  for(int i=0; i<50; i++){
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
}
