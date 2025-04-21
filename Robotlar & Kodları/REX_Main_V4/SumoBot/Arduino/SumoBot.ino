// ESP32 Kart Sürümü 2.0.11 olmalıdır

#define SensorLEFT 34   // Sol IR sensör pini
#define SensorRIGHT 35  // Sağ IR sensör pini
int trigPin = 17;       // Ultrasonik sensör - Trigger pini
int echoPin = 16;       // Ultrasonik sensör - Echo pini
long duration, cm;      // Ölçüm süresi ve mesafe için değişkenler

// Motor pin tanımlamaları
#define MotorA1 15
#define MotorA2 23

#define MotorB1 32
#define MotorB2 33

#define MotorC1 5
#define MotorC2 4

#define MotorD1 27
#define MotorD2 14

// Hız sabitleri
#define fast 255
#define mid 150
#define slow 100

#define THRESHOLD 3600  // IR sensörler için eşik değeri

// İleri hareket fonksiyonu (orta hız)
void forward() {
  analogWrite(MotorA1, mid);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, mid);
  analogWrite(MotorB2, 0);
  analogWrite(MotorC1, mid);
  analogWrite(MotorC2, 0);
  analogWrite(MotorD1, mid);
  analogWrite(MotorD2, 0);
}

// Yavaş ileri hareket fonksiyonu
void slowforward() {
  analogWrite(MotorA1, slow);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, slow);
  analogWrite(MotorB2, 0);
  analogWrite(MotorC1, slow);
  analogWrite(MotorC2, 0);
  analogWrite(MotorD1, slow);
  analogWrite(MotorD2, 0);
}

// Sağa dönüş fonksiyonu
void right() {
  analogWrite(MotorA1, mid);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, mid);
  analogWrite(MotorB2, 0);
  analogWrite(MotorC1, 0);
  analogWrite(MotorC2, mid);
  analogWrite(MotorD1, 0);
  analogWrite(MotorD2, mid);
}

// Sola dönüş fonksiyonu
void left() {
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, mid);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, mid);
  analogWrite(MotorC1, mid);
  analogWrite(MotorC2, 0);
  analogWrite(MotorD1, mid);
  analogWrite(MotorD2, 0);
}

// Robotu durdurma fonksiyonu
void stop() {
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, 0);
  analogWrite(MotorC1, 0);
  analogWrite(MotorC2, 0);
  analogWrite(MotorD1, 0);
  analogWrite(MotorD2, 0);
}

// Geri gitme fonksiyonu
void backward() {
  analogWrite(MotorA1, LOW);
  analogWrite(MotorA2, mid);
  analogWrite(MotorB1, LOW);
  analogWrite(MotorB2, mid);
  analogWrite(MotorC1, LOW);
  analogWrite(MotorC2, mid);
  analogWrite(MotorD1, LOW);
  analogWrite(MotorD2, mid);
}

// Başlangıç ayarları
void setup() {
  Serial.begin(115200);  // Seri iletişim başlatılıyor

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(SensorLEFT, INPUT);
  pinMode(SensorRIGHT, INPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);
}

// Ana döngü
void loop() {
  // Ultrasonik sensör ile mesafe ölçümü yapılır
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);      // Yankı süresi ölçülür
  cm = (duration / 2) / 29.1;             // Mesafe cm cinsine çevrilir
  delay(100);

  // IR sensörlerden veri okunur
  int leftSensor = analogRead(SensorLEFT);
  int rightSensor = analogRead(SensorRIGHT);

  // Her iki IR sensör de siyah çizgi algıladığında:
  if ((leftSensor < THRESHOLD && rightSensor < THRESHOLD)) {
    stop();         // Dur
    delay(500);
    backward();     // Geri git
    delay(800);
    left();         // Sola dön
    delay(400);
  }
  // Eğer mesafe 15 cm'den azsa ve çizgi algılanmıyorsa:
  else if ((cm < 15) && (leftSensor >= THRESHOLD && rightSensor >= THRESHOLD)) {
    forward();      // İleri git
  }
  // Normalde yavaşça ileri git
  else {
    slowforward();
  }
}
