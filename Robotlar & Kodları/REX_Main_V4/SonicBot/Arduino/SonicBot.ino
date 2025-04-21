//"""REX 8in1 Sonic Bot"""
// ESP32 Board Versiyonu 2.0.11 olmalıdır

int trigPin = 17;    // Ultrasonik sensör için trigger pini
int echoPin = 16;    // Ultrasonik sensör için echo pini
long duration, cm;   // Mesafe ölçümü için değişkenler

// Motor hızlarının tanımlanması
#define SLOW 120     // Yavaş hız
#define MID 140      // Orta hız
#define FAST 110     // Hızlı dönüş için kullanılacak değer

// Motor pinlerinin tanımlanması
#define MotorA1 15
#define MotorA2 23
#define MotorB1 32
#define MotorB2 33
#define MotorC1 5
#define MotorC2 4
#define MotorD1 27
#define MotorD2 14

// Buzzer pini tanımı
#define horn 25

// PWM özelliklerinin ayarlanması
const int freq = 50;  // PWM frekansı
const int resolution = 8;  // PWM çözünürlüğü (8-bit)

// Her motor için ayrı PWM kanalı tanımlanıyor
const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

void setup() {
  Serial.begin(115200);  // Seri monitör başlatılıyor

  // Ultrasonik sensör pin modları ayarlanıyor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Buzzer ve motor pinleri çıkış olarak ayarlanıyor
  pinMode(horn, OUTPUT);
  pinMode(MotorA1, OUTPUT); pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT); pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT); pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT); pinMode(MotorD2, OUTPUT);

  // Her motor pini için PWM kanalı yapılandırılıyor
  ledcSetup(PWMchannel_1, freq, resolution); ledcAttachPin(MotorA1, PWMchannel_1);
  ledcSetup(PWMchannel_2, freq, resolution); ledcAttachPin(MotorA2, PWMchannel_2);
  ledcSetup(PWMchannel_3, freq, resolution); ledcAttachPin(MotorB1, PWMchannel_3);
  ledcSetup(PWMchannel_4, freq, resolution); ledcAttachPin(MotorB2, PWMchannel_4);
  ledcSetup(PWMchannel_5, freq, resolution); ledcAttachPin(MotorC1, PWMchannel_5);
  ledcSetup(PWMchannel_6, freq, resolution); ledcAttachPin(MotorC2, PWMchannel_6);
  ledcSetup(PWMchannel_7, freq, resolution); ledcAttachPin(MotorD1, PWMchannel_7);
  ledcSetup(PWMchannel_8, freq, resolution); ledcAttachPin(MotorD2, PWMchannel_8);

  delay(1500);  // Başlangıçta bekleme süresi
}

void loop() {
  distance();  // Mesafe ölçülüyor

  if (cm < 8) { // Eğer engel 8 cm'den yakınsa:
    backward();  // Geri git
    digitalWrite(horn, HIGH);  // Buzzer çal
    delay(100);
    digitalWrite(horn, LOW);
    delay(1);
    left();      // Sola dön
    stop();      // Dur
    delay(100);
  } else {
    forward();   // Engel yoksa ileri git
  }
}

// İleri gitme fonksiyonu
void forward() {
  ledcWrite(PWMchannel_1, MID);
  ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID);
  ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, MID);
  ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID);
  ledcWrite(PWMchannel_8, LOW);
}

// Sağa dönme fonksiyonu
void right() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, FAST);
  ledcWrite(PWMchannel_3, FAST);
  ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW);
  ledcWrite(PWMchannel_6, FAST);
  ledcWrite(PWMchannel_7, LOW);
  ledcWrite(PWMchannel_8, FAST);
  delay(1000);
}

// Sola dönme fonksiyonu
void left() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW);
  ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, MID);
  ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID);
  ledcWrite(PWMchannel_8, LOW);
  delay(850);
}

// Robotu durdurma fonksiyonu
void stop() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, LOW);
  ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW);
  ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, LOW);
  ledcWrite(PWMchannel_8, LOW);
}

// Geri gitme fonksiyonu
void backward() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, SLOW);
  ledcWrite(PWMchannel_3, LOW);
  ledcWrite(PWMchannel_4, SLOW);
  ledcWrite(PWMchannel_5, LOW);
  ledcWrite(PWMchannel_6, SLOW);
  ledcWrite(PWMchannel_7, LOW);
  ledcWrite(PWMchannel_8, SLOW);
  delay(200);
}

// Ultrasonik sensör ile mesafe ölçme fonksiyonu
void distance() {
  delay(40);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);     // 10 mikro saniyelik sinyal gönderilir
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);  // Yankı süresi ölçülür
  cm = (duration / 2) / 29.1;         // cm cinsine çevrilir
}
