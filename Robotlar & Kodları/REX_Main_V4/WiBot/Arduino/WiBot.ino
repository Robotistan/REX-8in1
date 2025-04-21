// ESP32 Kart Sürümü 2.0.11 olmalıdır

// Wi-Bot için Dabble kütüphanesini tanımla
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Arduino.h>

// Motor pinleri tanımlanıyor (ileri ve geri hareket için)
#define MotorA1 15  // A motor ileri
#define MotorA2 23  // A motor geri

#define MotorB1 32  // B motor ileri
#define MotorB2 33  // B motor geri

#define MotorC1 5   // C motor ileri
#define MotorC2 4   // C motor geri

#define MotorD1 27  // D motor ileri
#define MotorD2 14  // D motor geri

// Buzzer pini (korna)
#define horn 25

void setup() {
  // Pin modları tanımlanıyor
  pinMode(horn, OUTPUT);

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

  // Dabble uygulaması başlatılıyor ve Bluetooth ismi atanıyor
  Dabble.begin("REX_ROBOT");
}

void loop() {
  // Dabble'dan gelen komutlar işleniyor
  Dabble.processInput();

  // Her döngüde önce robot durduruluyor
  stop();

  // Kumandadan gelen yön tuşlarına göre robot yönlendiriliyor
  if (GamePad.isUpPressed()) forward();
  if (GamePad.isDownPressed()) backward();
  if (GamePad.isLeftPressed()) left();
  if (GamePad.isRightPressed()) right();

  // Dörtgen butonuna basıldığında konsola yazı gönderilir
  if (GamePad.isSquarePressed()) {
    Serial.print("Square");
  }

  // Yuvarlak butonuna basıldığında hareket kombinasyonu ve buzzer çalıştırılır
  if (GamePad.isCirclePressed()) {
    for (int i = 0; i < 3; i++) {
      forward();
      digitalWrite(horn, HIGH);
      delay(300);
      digitalWrite(horn, LOW);

      left();
      digitalWrite(horn, HIGH);
      delay(300);
      digitalWrite(horn, LOW);

      right();
      digitalWrite(horn, HIGH);
      delay(300);
      digitalWrite(horn, LOW);

      left();
      digitalWrite(horn, HIGH);
      delay(300);
      digitalWrite(horn, LOW);
    }
  }

  // Çarpı butonuna basıldığında buzzer kısa süreli çalar
  if (GamePad.isCrossPressed()) {
    Serial.print("Cross");
    digitalWrite(horn, HIGH);
    delay(100);
    digitalWrite(horn, LOW);
  }

  // Üçgen butonuna basıldığında mesaj basılır
  if (GamePad.isTrianglePressed()) {
    Serial.print("Triangle");
  }

  if (GamePad.isStartPressed()) {
    Serial.print("Start");
  }

  if (GamePad.isSelectPressed()) {
    Serial.print("Select");
  }

  // Joystick değerleri seri porta yazdırılır
  Serial.print('\t');
  int a = GamePad.getAngle();
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');

  int b = GamePad.getRadius();
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');

  float c = GamePad.getXaxisData();
  Serial.print("x_axis: ");
  Serial.print(c);
  Serial.print('\t');

  float d = GamePad.getYaxisData();
  Serial.print("y_axis: ");
  Serial.println(d);
  Serial.println();
}

// Robot ileri hareket fonksiyonu
void forward() {
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

// Robot sağa dönüş fonksiyonu
void right() {
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

// Robot sola dönüş fonksiyonu
void left() {
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}

// Robot durdurma fonksiyonu
void stop() {
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, LOW);
}

// Robot geri hareket fonksiyonu
void backward() {
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}
