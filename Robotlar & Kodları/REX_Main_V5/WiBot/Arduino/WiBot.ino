// ESP32 kart sürümünüzün 2.0.11 olması gereklidir

// Wi-Bot kontrolü için gerekli ayarlar
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>   // Dabble kütüphanesi (Bluetooth üzerinden kontrol için)
#include <Arduino.h>

// Motor pinleri tanımlanıyor
#define MotorPWM 13	// PWM sinyali için motor anahtarlama pini

#define MotorA1 15  // Motor A ileri
#define MotorA2 23  // Motor A geri

#define MotorB1 32  // Motor B ileri
#define MotorB2 33  // Motor B geri

#define MotorC1 27  // Motor C ileri
#define MotorC2 14  // Motor C geri

#define MotorD1 5   // Motor D ileri
#define MotorD2 4   // Motor D geri

// Buzzer (korna) pini
#define horn 25

void setup() {
  // Giriş/Çıkış pinleri ayarlanıyor
  pinMode(horn, OUTPUT);
  pinMode(MotorPWM, OUTPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);

  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  Serial.begin(115200);             // Seri monitör başlatılıyor
  Dabble.begin("REX_ROBOT");        // Bluetooth cihaz ismi belirleniyor
}

void loop() {
  Dabble.processInput();   // Bluetooth üzerinden gelen komutlar işleniyor
  stop();                  // Her döngüde motorlar önce durduruluyor

  // Yön tuşlarına göre hareket belirleniyor
  if (GamePad.isUpPressed()) forward();
  if (GamePad.isDownPressed()) backward();
  if (GamePad.isLeftPressed()) left();
  if (GamePad.isRightPressed()) right();

  // Özel butonlar için tepkiler
  if (GamePad.isSquarePressed()) Serial.print("Square");
  if (GamePad.isCirclePressed()) rex_horn();        // Korna çal
  if (GamePad.isCrossPressed()) rex_horn();         // Korna çal
  if (GamePad.isTrianglePressed()) Serial.print("Triangle");
  if (GamePad.isStartPressed()) Serial.print("Start");
  if (GamePad.isSelectPressed()) Serial.print("Select");

  // Joystick verileri seri porta yazdırılıyor
  Serial.print('\t');
  int a = GamePad.getAngle();       // Joystick açısı
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');

  int b = GamePad.getRadius();      // Joystick uzaklığı (hareket şiddeti)
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');

  float c = GamePad.getXaxisData(); // X ekseni (sağ-sol)
  Serial.print("x_axis: ");
  Serial.print(c);
  Serial.print('\t');

  float d = GamePad.getYaxisData(); // Y ekseni (ileri-geri)
  Serial.print("y_axis: ");
  Serial.println(d);
  Serial.println();
}

// İleri hareket fonksiyonu
void forward() { 
  digitalWrite(MotorPWM, HIGH);  // PWM sinyali aktif

  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

// Sola dönüş fonksiyonu
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

// Sağa dönüş fonksiyonu
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

// Robotu durdurma fonksiyonu
void stop() {
  digitalWrite(MotorPWM, LOW);   // PWM sinyali kapatılır

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, LOW);
}

// Geri hareket fonksiyonu
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

// Korna fonksiyonu - kısa titreşimli ses çıkarır
void rex_horn() {
  for(int i = 0; i < 50; i++) {
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
}
