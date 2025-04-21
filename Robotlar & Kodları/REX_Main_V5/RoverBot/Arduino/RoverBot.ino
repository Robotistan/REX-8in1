//"""REX 8in1 Rover Bot"""
// Bu kod ESP32 kartı için yazılmıştır. Versiyon: 2.0.11

#include <DabbleESP32.h>  // Dabble kütüphanesi Bluetooth kontrolü için
#include <Arduino.h>      // Temel Arduino fonksiyonları

// Motor pinlerinin tanımlanması
#define MotorPWM 13 // PWM sinyali çıkışı (motorları genel olarak aktif etmek için)

// Motor A - ileri ve geri pinleri
#define MotorA1 15 // İleri
#define MotorA2 23 // Geri

// Motor B - ileri ve geri pinleri
#define MotorB1 32 // İleri
#define MotorB2 33 // Geri

// Motor C - ileri ve geri pinleri
#define MotorC1 5  // İleri
#define MotorC2 4  // Geri

// Motor D - ileri ve geri pinleri
#define MotorD1 27 // İleri
#define MotorD2 14 // Geri

// Buzzer pini
#define horn 25

void setup() {
  pinMode(horn, OUTPUT);        // Buzzer çıkışı
  pinMode(MotorPWM, OUTPUT);    // PWM çıkışı

  // Tüm motor pinlerini çıkış olarak ayarla
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);
  
  Serial.begin(115200);  // Seri haberleşmeyi başlat (debug için)
  Dabble.begin("REX_ROBOT"); // Bluetooth cihaz ismini tanımla
}

void loop() {
  Dabble.processInput(); // Dabble'dan gelen veriyi işle
  stop(); // Her döngü başında durdurma komutu (güvenlik amaçlı)

  // Gamepad yön tuşlarına göre hareket komutları
  if (GamePad.isUpPressed()) {
    forward();
  }

  if (GamePad.isDownPressed()) {
    backward();
  }

  if (GamePad.isLeftPressed()) {
    left();
  }

  if (GamePad.isRightPressed()) {
    right();
  }

  // Diğer butonlara basıldığında seri monitöre yazı yazdır
  if (GamePad.isSquarePressed()) {
    Serial.print("Square");
  }

  if (GamePad.isCirclePressed()) {
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed()) {
    rex_horn();  // Buzzer'ı çalıştır
  }

  if (GamePad.isTrianglePressed()) {
    Serial.print("Triangle");
  }

  if (GamePad.isStartPressed()) {
    Serial.print("Start");
  }

  if (GamePad.isSelectPressed()) {
    Serial.print("Select");
  }
}

// İleri gitme fonksiyonu
void forward() { 
  digitalWrite(MotorPWM, HIGH); // Motorları aktif et

  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

// Sağa dönme fonksiyonu
void right() { 
  digitalWrite(MotorPWM, HIGH);

  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}

// Sola dönme fonksiyonu
void left() { 
  digitalWrite(MotorPWM, HIGH);

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

// Robotu durdurma fonksiyonu
void stop() {
  digitalWrite(MotorPWM, LOW); // Motorları devre dışı bırak

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, LOW);
}

// Geri gitme fonksiyonu
void backward() { 
  digitalWrite(MotorPWM, HIGH);
  
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}

// Buzzer çalma fonksiyonu
void rex_horn() {
  for(int i=0; i<50; i++){
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
}
