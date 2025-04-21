// """REX 8in1 Rover Bot"""
// ESP32 Kart Versiyonu 2.0.11 olmalıdır

#include <DabbleESP32.h>  // Dabble kütüphanesi ile Bluetooth üzerinden telefonla haberleşmeyi sağlar
#include <Arduino.h>

// Motor pinlerini tanımlıyoruz
#define MotorA1 15 // A motoru ileri
#define MotorA2 23 // A motoru geri

#define MotorB1 32 // B motoru ileri
#define MotorB2 33 // B motoru geri

#define MotorC1 5  // C motoru ileri
#define MotorC2 4  // C motoru geri

#define MotorD1 27 // D motoru ileri
#define MotorD2 14 // D motoru geri

// Buzzer pinini tanımlıyoruz
#define horn 25

void setup() {
  // Buzzer çıkışı ayarlanıyor
  pinMode(horn, OUTPUT);

  // Tüm motor pinleri çıkış olarak ayarlanıyor
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);
  
  // Seri haberleşme başlatılıyor
  Serial.begin(115200);  

  // Bluetooth cihaz adı tanımlanıyor
  Dabble.begin("REX_ROBOT"); 
}

void loop() {
  Dabble.processInput(); // Dabble üzerinden gelen veriler okunuyor
  stop(); // Her döngü başında motorlar durduruluyor (butona basılmadıysa hareket etmesin)

  // GamePad yön butonları ile robot yönlendirilir
  if (GamePad.isUpPressed()) {
    forward();  // İleri git
  }

  if (GamePad.isDownPressed()) {
    backward(); // Geri git
  }

  if (GamePad.isLeftPressed()) {
    left();     // Sola dön
  }

  if (GamePad.isRightPressed()) {
    right();    // Sağa dön
  }

  // Diğer GamePad butonlarına basıldığında sadece bilgi yazdırılır
  if (GamePad.isSquarePressed()) {
    Serial.print("Square");
  }

  if (GamePad.isCirclePressed()) {
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed()) {
    Serial.print("Cross");
    digitalWrite(horn, HIGH);  // Buzzer çal
    delay(100);
    digitalWrite(horn, LOW);   // Buzzer durdur
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

// İleri hareket fonksiyonu - tüm motorları ileri yönde döndürür
void forward() {
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Sağa dönüş fonksiyonu
void right() {
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
}

// Sola dönüş fonksiyonu
void left() {
  digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Tüm motorları durdurur
void stop() {
  digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, LOW);
}

// Geri hareket fonksiyonu
void backward() {
  digitalWrite(MotorA1, LOW);  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW);  digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, LOW);  digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW);  digitalWrite(MotorD2, HIGH);
}
