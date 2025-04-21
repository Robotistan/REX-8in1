// """REX 8in1 Tracker Bot"""
// ESP32 kart sürümü 2.0.11 olmalı

#define left_sensor 34   // Sol IR sensörü analog pin
#define right_sensor 35  // Sağ IR sensörü analog pin

// Motor pin tanımlamaları
#define MotorA1 15
#define MotorA2 23

#define MotorB1 32
#define MotorB2 33

#define MotorC1 5
#define MotorC2 4

#define MotorD1 27
#define MotorD2 14

// Motor hızı (PWM değeri)
#define MID 140

// Hareket yönleri
#define STOP 0   // Dur
#define FWD 1    // İleri
#define BWD 2    // Geri
#define RIGHT 3  // Sağa dön
#define LEFT 4   // Sola dön

// IR sensör eşik değeri (çizgi ile arka planı ayırt etmek için)
#define THRESHOLD 1200

// PWM ayarları
const int freq = 50;  // PWM frekansı
const int resolution = 8;  // PWM çözünürlüğü (8 bit = 0-255 arası)
const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

// Yön durumu değişkenleri
uint8_t directionStt = STOP;     // Şu anki yön
uint8_t oldDirection = STOP;     // Önceki yön

unsigned long reverseTime = 0;   // Geri gitme başlangıç zamanı

void setup() {
  Serial.begin(115200);  // Seri iletişimi başlat

  // IR sensör pinlerini giriş olarak ayarla
  pinMode(left_sensor, INPUT);
  pinMode(right_sensor, INPUT);

  // Motor pinlerini çıkış olarak ayarla
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  // Motor pinleri için PWM yapılandırması
  ledcSetup(PWMchannel_1, freq, resolution);
  ledcAttachPin(MotorA1, PWMchannel_1);

  ledcSetup(PWMchannel_2, freq, resolution);
  ledcAttachPin(MotorA2, PWMchannel_2);

  ledcSetup(PWMchannel_3, freq, resolution);
  ledcAttachPin(MotorB1, PWMchannel_3);

  ledcSetup(PWMchannel_4, freq, resolution);
  ledcAttachPin(MotorB2, PWMchannel_4);

  ledcSetup(PWMchannel_5, freq, resolution);
  ledcAttachPin(MotorC1, PWMchannel_5);

  ledcSetup(PWMchannel_6, freq, resolution);
  ledcAttachPin(MotorC2, PWMchannel_6);

  ledcSetup(PWMchannel_7, freq, resolution);
  ledcAttachPin(MotorD1, PWMchannel_7);

  ledcSetup(PWMchannel_8, freq, resolution);
  ledcAttachPin(MotorD2, PWMchannel_8);
}

void loop() {
  // IR sensörlerden veri oku
  int leftSensor = analogRead(left_sensor);
  int rightSensor = analogRead(right_sensor);

  Serial.print("left:");
  Serial.print(leftSensor);
  Serial.print("  right:");
  Serial.println(rightSensor);
  Serial.println("");

  // Her iki sensör de çizgiyi görüyorsa ileri git
  if (leftSensor >= THRESHOLD && rightSensor >= THRESHOLD) {
    directionStt = FWD;
  } 
  // Sadece sağ sensör çizgiyi görüyorsa sağa dön
  else if (leftSensor < THRESHOLD && rightSensor > THRESHOLD) {
    directionStt = RIGHT;
  } 
  // Sadece sol sensör çizgiyi görüyorsa sola dön
  else if (leftSensor > THRESHOLD && rightSensor < THRESHOLD) {
    directionStt = LEFT;
  } 
  // Hiçbir sensör çizgiyi görmüyorsa geri git
  else if (leftSensor < THRESHOLD && rightSensor < THRESHOLD && directionStt != STOP) {
    directionStt = BWD;
  }

  // Yön değiştiyse yeni yönü uygula
  if (directionStt != oldDirection) {
    oldDirection = directionStt;
    if (directionStt == FWD)
      forward();
    else if (directionStt == RIGHT)
      right();
    else if (directionStt == LEFT)
      left();
    else if (directionStt == BWD) {
      backward();
      reverseTime = millis();  // Geri hareket süresi başlatılır
    } else if (directionStt == STOP)
      stop();
  }

  // Geri gittikten 300ms sonra dur
  if (directionStt == BWD && millis() - reverseTime > 300)
    directionStt = STOP;
}

// İleri hareket
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

// Sağa dönüş
void right() {
  ledcWrite(PWMchannel_1, MID);
  ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID);
  ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW);
  ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW);
  ledcWrite(PWMchannel_8, MID);
}

// Sola dönüş
void left() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW);
  ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, MID);
  ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID);
  ledcWrite(PWMchannel_8, LOW);
}

// Dur
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

// Geri git
void backward() {
  ledcWrite(PWMchannel_1, LOW);
  ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW);
  ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, LOW);
  ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW);
  ledcWrite(PWMchannel_8, MID);
}
