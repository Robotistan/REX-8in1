//"""REX 8in1 Sonic Bot"""
// ESP32 kart sürümü 2.0.11 olmalıdır

// Ultrasonik sensör pinleri
int trigPin = 17;    // Trigger pini
int echoPin = 16;    // Echo pini
long duration, cm;   // Mesafe ölçüm değişkenleri

// Motor hız değerleri
#define SLOW 120
#define MID 140
#define FAST 110

// Motor pinleri
#define MotorPWM 13 // PWM çıkış pini

#define MotorA1 15
#define MotorA2 23

#define MotorB1 32
#define MotorB2 33

#define MotorC1 5
#define MotorC2 4

#define MotorD1 14
#define MotorD2 27

// Buzzer pini (korna)
#define horn 25

// PWM ayarları
const int freq = 50;
const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

const int resolution = 8; // 8 bit çözünürlük

int turns = 0 ; // Engelden kaçınma sayacı

void setup() {
  Serial.begin(115200); // Seri haberleşme başlatılır

  // Ultrasonik sensör pin modları
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(horn, OUTPUT);  // Buzzer çıkışı

  pinMode(MotorPWM, OUTPUT); // PWM motor çıkışı

  // Motor pinleri çıkış olarak ayarlanır
  pinMode(MotorA1, OUTPUT); pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT); pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT); pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT); pinMode(MotorD2, OUTPUT);

  // Motorlar için PWM kanalları yapılandırılır
  ledcSetup(PWMchannel_1, freq, resolution); ledcAttachPin(MotorA1, PWMchannel_1);
  ledcSetup(PWMchannel_2, freq, resolution); ledcAttachPin(MotorA2, PWMchannel_2);
  ledcSetup(PWMchannel_3, freq, resolution); ledcAttachPin(MotorB1, PWMchannel_3);
  ledcSetup(PWMchannel_4, freq, resolution); ledcAttachPin(MotorB2, PWMchannel_4);
  ledcSetup(PWMchannel_5, freq, resolution); ledcAttachPin(MotorC1, PWMchannel_5);
  ledcSetup(PWMchannel_6, freq, resolution); ledcAttachPin(MotorC2, PWMchannel_6);
  ledcSetup(PWMchannel_7, freq, resolution); ledcAttachPin(MotorD1, PWMchannel_7);
  ledcSetup(PWMchannel_8, freq, resolution); ledcAttachPin(MotorD2, PWMchannel_8);

  delay(1500); // Başlangıç gecikmesi
}

void loop() {
  Serial.println(cm);   // Ölçülen mesafeyi seri porta yaz
  distance();           // Mesafe ölçümünü gerçekleştir
  delay(500);           // Ölçüm aralığı

  if (cm < 8) {         // Eğer 8 cm'den daha yakın bir cisim varsa
    backward();         // Geri git
    rex_horn();         // Buzzer çal
    left();             // Sola dön
    stop();             // Dur
    delay(100);
    turns++;            // Dönüş sayısını arttır
  } else {
    forward();          // Yolu açıksa ileri git
  }

  if (turns > 2) {      // 2'den fazla engelle karşılaştıysa dur
    stop();
  }
}

// İleri gitme fonksiyonu
void forward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, MID); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
}

// Sağ dönüş fonksiyonu
void right() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, FAST);
  ledcWrite(PWMchannel_3, FAST); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, FAST);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, FAST);
  delay(1000);
}

// Sol dönüş fonksiyonu
void left() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
  delay(850);
}

// Durma fonksiyonu
void stop() {
  digitalWrite(MotorPWM, LOW);
  for (int i = 1; i <= 8; i++) {
    ledcWrite(i, LOW); // Tüm PWM kanallarına LOW sinyal gönder
  }
}

// Geri gitme fonksiyonu
void backward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, SLOW);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, SLOW);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, SLOW);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, SLOW);
  delay(200);
}

// Ultrasonik mesafe ölçüm fonksiyonu
void distance() {
  delay(40);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Echo pininden gelen sinyali oku
  cm = (duration / 2) / 29.1;        // cm cinsinden hesapla
}

// Buzzer çalma fonksiyonu
void rex_horn() {
  for (int i = 0; i < 50; i++) {
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
}
