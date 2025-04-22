//"""REX 8in1 Tracker Bot"""
// ESP32 kart sürümünüzün 2.0.11 olması gerekmektedir.

#define left_sensor 34   // Sol IR sensör pini (analog giriş)
#define right_sensor 35  // Sağ IR sensör pini (analog giriş)

// Motor pinleri ve hız kontrol pini tanımlanıyor
#define MotorPWM 13	// PWM kontrol pini

#define MotorA1 15  // Motor A ileri
#define MotorA2 23  // Motor A geri

#define MotorB1 32  // Motor B ileri
#define MotorB2 33  // Motor B geri

#define MotorC1 5   // Motor C ileri
#define MotorC2 4   // Motor C geri

#define MotorD1 27  // Motor D ileri
#define MotorD2 14  // Motor D geri

// Motor hızı
#define MID 140  // Orta hız

// Hareket yönleri için sabitler
#define STOP 0
#define FWD 1
#define BWD 2
#define RIGHT 3
#define LEFT 4

// IR sensör için eşik değeri
#define THRESHOLD 1200

// PWM özellikleri tanımlanıyor
const int freq = 50;             // Frekans
const int resolution = 8;        // Çözünürlük (8-bit)
const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

// Yön durumu takibi
uint8_t directionStt = STOP;
uint8_t oldDirection = STOP;

// Geri viteste ne kadar durulduğunu takip etmek için zamanlayıcı
unsigned long reverseTime = 0;

void setup() {
  Serial.begin(115200); // Seri monitör başlatılıyor

  // Sensör pinleri giriş olarak ayarlanıyor
  pinMode(left_sensor, INPUT);
  pinMode(right_sensor, INPUT);

  // Motor pinleri çıkış olarak ayarlanıyor
  pinMode(MotorPWM, OUTPUT);

  pinMode(MotorA1, OUTPUT); pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT); pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT); pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT); pinMode(MotorD2, OUTPUT);

  // PWM kanalları tanımlanıyor ve pinlere atanıyor
  ledcSetup(PWMchannel_1, freq, resolution); ledcAttachPin(MotorA1, PWMchannel_1);
  ledcSetup(PWMchannel_2, freq, resolution); ledcAttachPin(MotorA2, PWMchannel_2);
  ledcSetup(PWMchannel_3, freq, resolution); ledcAttachPin(MotorB1, PWMchannel_3);
  ledcSetup(PWMchannel_4, freq, resolution); ledcAttachPin(MotorB2, PWMchannel_4);
  ledcSetup(PWMchannel_5, freq, resolution); ledcAttachPin(MotorC1, PWMchannel_5);
  ledcSetup(PWMchannel_6, freq, resolution); ledcAttachPin(MotorC2, PWMchannel_6);
  ledcSetup(PWMchannel_7, freq, resolution); ledcAttachPin(MotorD1, PWMchannel_7);
  ledcSetup(PWMchannel_8, freq, resolution); ledcAttachPin(MotorD2, PWMchannel_8);
}

void loop() {
  // IR sensörlerinden değerleri oku
  int leftSensor = analogRead(left_sensor);
  int rightSensor = analogRead(right_sensor);

  // Değerleri seri monitöre yazdır
  Serial.print("left:"); Serial.print(leftSensor);
  Serial.print("  right:"); Serial.println(rightSensor);
  Serial.println("");

  // Her iki sensör de siyah çizgiyi görüyorsa ileri git
  if (leftSensor >= THRESHOLD && rightSensor >= THRESHOLD) {
    directionStt = FWD;
  } 
  // Sadece sağ sensör siyah çizgideyse sağa dön
  else if (leftSensor < THRESHOLD && rightSensor > THRESHOLD) {
    directionStt = RIGHT;
  } 
  // Sadece sol sensör siyah çizgideyse sola dön
  else if (leftSensor > THRESHOLD && rightSensor < THRESHOLD) {
    directionStt = LEFT;
  } 
  // Her iki sensör de çizgide değilse geri git
  else if (leftSensor < THRESHOLD && rightSensor < THRESHOLD && directionStt != STOP) {
    directionStt = BWD;
  }

  // Eğer yön değiştiyse, yeni yöne uygun komutu çalıştır
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
      reverseTime = millis(); // Geri gitme süresini başlat
    } else if (directionStt == STOP)
      stop();
  }

  // Geri gitme süresi 300ms'i geçtiyse dur
  if (directionStt == BWD && millis() - reverseTime > 300)
    directionStt = STOP;
}

// İleri hareket fonksiyonu
void forward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, MID); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
}

// Sağa dönüş fonksiyonu
void right() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, MID); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, MID);
}

// Sola dönüş fonksiyonu
void left() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
}

// Robotu tamamen durdurur
void stop() {
  digitalWrite(MotorPWM, LOW);
  for (int i = PWMchannel_1; i <= PWMchannel_8; i++) {
    ledcWrite(i, LOW);
  }
}

// Geri gitme fonksiyonu
void backward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, MID);
}
