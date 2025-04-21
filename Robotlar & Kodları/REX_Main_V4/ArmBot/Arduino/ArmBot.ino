//"""REX 8in1 Arm Bot"""
// ESP32 Board Versiyonu 2.0.11 olmalıdır

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>       // Dabble kütüphanesi - GamePad kontrolü için
#include <Arduino.h>
#include <ESP32Servo.h>        // Servo motor kontrol kütüphanesi

// Motor tipi enum'u tanımlanıyor: DC motor ya da Servo motor
enum MOTOR_TYPE {
  DC_MOTOR,
  SERVO_MOTOR
};

enum MOTOR_TYPE motorType = DC_MOTOR;  // Varsayılan motor tipi DC

// DC Motor Pin Tanımlamaları
#define MotorA1 15
#define MotorA2 23
#define MotorB1 32
#define MotorB2 33
#define MotorC1 5
#define MotorC2 4
#define MotorD1 27
#define MotorD2 14

// Servo motorların başlangıç pozisyonları
int position1 = 90;
int position2 = 90;
int position3 = 90;
int position4 = 90;

// Buzzer pini
int horn = 25;

// Servo motor tanımlamaları
Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

void setup() {
  // PWM zamanlayıcılarının ayrılması
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Servo motorlar için frekans ayarı
  Servo1.setPeriodHertz(50);
  Servo2.setPeriodHertz(50);
  Servo3.setPeriodHertz(50);
  Servo4.setPeriodHertz(50);

  pinMode(horn, OUTPUT);  // Buzzer çıkışı

  // Servo motor pin bağlantıları
  Servo1.attach(2, 600, 2500);
  Servo2.attach(26, 600, 2500);
  Servo3.attach(18, 600, 2500);
  Servo4.attach(19, 600, 2500);

  // Başlangıç pozisyonlarına getiriliyor
  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);

  // DC motor pinleri çıkış olarak ayarlanıyor
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
  
  // Bluetooth adı tanımlanıyor
  Dabble.begin("REX_ROBOT");
}

void loop() {
  Dabble.processInput();  // GamePad'den gelen veriler işleniyor
  stop();                 // Motorlar durduruluyor

  // SELECT tuşuna basıldığında Servo moda geçiliyor
  if (GamePad.isSelectPressed()) {
    motorType = SERVO_MOTOR;
    digitalWrite(horn, HIGH);
    delay(100);
    digitalWrite(horn, LOW);
    delay(1);

    int position1 = 90;
    int position2 = 90;
    int position3 = 90;
    int position4 = 90;
  }

  // START tuşuna basıldığında DC moda geçiliyor
  if (GamePad.isStartPressed()) {
    motorType = DC_MOTOR;
    digitalWrite(horn, HIGH);
    delay(100);
    digitalWrite(horn, LOW);
    delay(1);
    digitalWrite(horn, HIGH);
    delay(100);
    digitalWrite(horn, LOW);
    delay(1);
  }

  // Motor tipi değişkenine göre ilgili fonksiyonlar çağrılıyor
  switch (motorType) {
    case DC_MOTOR:
      dc_motor();
      break;
    case SERVO_MOTOR:
      servo_motor();
      break;
  }

  // Açıyı, yönü ve joystick verilerini okuyabilirsin (şu an yorum satırı)
}

void servo_motor() {
  // GamePad yön tuşlarına göre servo motor pozisyonları ayarlanıyor
  if (GamePad.isUpPressed() && position2 > 0) position2--;
  if (GamePad.isDownPressed() && position2 < 140) position2++;
  if (GamePad.isRightPressed() && position1 < 140) position1++;
  if (GamePad.isLeftPressed() && position1 > 40) position1--;

  if (GamePad.isSquarePressed() && position4 < 160) position4++;
  if (GamePad.isCirclePressed() && position4 > 90) position4--;
  if (GamePad.isCrossPressed() && position3 > 30) position3--;
  if (GamePad.isTrianglePressed() && position3 < 150) position3++;

  delay(10);  // Küçük bir bekleme

  // Güncel pozisyonlar servo motorlara gönderiliyor
  Servo1.write(position1);
  Servo2.write(position2);
  Servo3.write(position3);
  Servo4.write(position4);
}

void dc_motor() {
  // Yön tuşlarına göre hareketler
  if (GamePad.isUpPressed()) forward();
  if (GamePad.isDownPressed()) backward();
  if (GamePad.isLeftPressed()) left();
  if (GamePad.isRightPressed()) right();

  // SQUARE tuşu boş bırakılmış

  // CIRCLE tuşuna basıldığında özel hareketler ve ses efekti
  if (GamePad.isCirclePressed()) {
    for (int i = 0; i < 3; i++) {
      forward(); digitalWrite(horn, HIGH); delay(300); digitalWrite(horn, LOW); delay(300);
      left(); digitalWrite(horn, HIGH); delay(400); digitalWrite(horn, LOW); delay(300);
      right(); digitalWrite(horn, HIGH); delay(500); digitalWrite(horn, LOW); delay(300);
      digitalWrite(horn, HIGH); delay(600); digitalWrite(horn, LOW);
      left(); delay(300);
    }
  }

  // CROSS tuşu -> korna çalar
  if (GamePad.isCrossPressed()) {
    digitalWrite(horn, HIGH);
    delay(200);
    digitalWrite(horn, LOW);
  }

  // TRIANGLE tuşu -> tüm servo motorları ortalar
  if (GamePad.isTrianglePressed()) {
    Servo1.write(90);
    Servo2.write(90);
    Servo3.write(90);
    Servo4.write(90);
  }
}

// İleri yönde hareket
void forward() {
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Sağ dönüş
void right() {
  digitalWrite(MotorA1, HIGH); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
}

// Sol dönüş
void left() {
  digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, HIGH); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, HIGH); digitalWrite(MotorD2, LOW);
}

// Tüm motorları durdur
void stop() {
  digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, LOW);
  digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, LOW);
}

// Geri git
void backward() {
  digitalWrite(MotorA1, LOW); digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW); digitalWrite(MotorB2, HIGH);
  digitalWrite(MotorC1, LOW); digitalWrite(MotorC2, HIGH);
  digitalWrite(MotorD1, LOW); digitalWrite(MotorD2, HIGH);
}
