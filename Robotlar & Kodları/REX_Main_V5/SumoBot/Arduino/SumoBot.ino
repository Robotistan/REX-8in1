//"""REX 8in1 Sumo Bot"""
// Robotlar hakkında bilgi için: https://rex-rdt.readthedocs.io/en/latest/

#define SensorLEFT 34   // Sol IR sensör pini
#define SensorRIGHT 35  // Sağ IR sensör pini

// Motor pinleri tanımlanıyor
#define MotorPWM 13	    // PWM sinyal pini (motorları çalıştırmak için)

#define MotorA1 15       // Motor A ileri
#define MotorA2 23       // Motor A geri

#define MotorB1 32       // Motor B ileri
#define MotorB2 33       // Motor B geri

#define MotorC1 5        // Motor C ileri
#define MotorC2 4        // Motor C geri

#define MotorD1 27       // Motor D ileri
#define MotorD2 14       // Motor D geri

// Motor hızları
#define FAST 255
#define MID 130
#define SLOW 100

// IR sensör eşiği
#define THRESHOLD 3600

// Ultrasonik sensör pinleri
int trigPin = 17;   // Trigger pini
int echoPin = 16;   // Echo pini
long duration, cm;  // Mesafe hesaplaması için değişkenler

// PWM ayarları
const int freq = 50; // PWM frekansı
const int resolution = 8; // PWM çözünürlüğü (8-bit)

const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

void setup() {
  Serial.begin(115200); // Seri iletişimi başlat

  // Sensör pin ayarları
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(SensorLEFT, INPUT);
  pinMode(SensorRIGHT, INPUT);

  pinMode(MotorPWM, OUTPUT);

  // Motor pinleri çıkış olarak ayarlanıyor
  pinMode(MotorA1, OUTPUT); pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT); pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT); pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT); pinMode(MotorD2, OUTPUT);

  // PWM kanallarını motor pinlerine bağla
  ledcSetup(PWMchannel_1, freq, resolution); ledcAttachPin(MotorA1, PWMchannel_1);
  ledcSetup(PWMchannel_2, freq, resolution); ledcAttachPin(MotorA2, PWMchannel_2);
  ledcSetup(PWMchannel_3, freq, resolution); ledcAttachPin(MotorB1, PWMchannel_3);
  ledcSetup(PWMchannel_4, freq, resolution); ledcAttachPin(MotorB2, PWMchannel_4);
  ledcSetup(PWMchannel_5, freq, resolution); ledcAttachPin(MotorC1, PWMchannel_5);
  ledcSetup(PWMchannel_6, freq, resolution); ledcAttachPin(MotorC2, PWMchannel_6);
  ledcSetup(PWMchannel_7, freq, resolution); ledcAttachPin(MotorD1, PWMchannel_7);
  ledcSetup(PWMchannel_8, freq, resolution); ledcAttachPin(MotorD2, PWMchannel_8);
}

// Robotu ileri hareket ettir
void forward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, MID); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
}

// Robotu sağa döndür
void right() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, MID); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, MID); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, MID);
}

// Robotu sola döndür
void left() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, MID); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, MID); ledcWrite(PWMchannel_8, LOW);
}

// Robotu durdur
void stop() {
  digitalWrite(MotorPWM, LOW);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, LOW);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, LOW);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, LOW);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, LOW);
}

// Robotu geri hareket ettir
void backward() {
  digitalWrite(MotorPWM, HIGH);
  ledcWrite(PWMchannel_1, LOW); ledcWrite(PWMchannel_2, MID);
  ledcWrite(PWMchannel_3, LOW); ledcWrite(PWMchannel_4, MID);
  ledcWrite(PWMchannel_5, LOW); ledcWrite(PWMchannel_6, MID);
  ledcWrite(PWMchannel_7, LOW); ledcWrite(PWMchannel_8, MID);
}

// Ana döngü
void loop() {
  // Mesafe ölçümü yapılır
  delay(40);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 2) / 29.1;

  // Engel mesafesi kontrolü
  if(cm <= 15){
    int leftSensor = analogRead(SensorLEFT);
    int rightSensor = analogRead(SensorRIGHT);
    delay(2);
    
    // Eğer çizgi algılandıysa geri git
    if ((leftSensor >= THRESHOLD) || (rightSensor >= THRESHOLD)) {
      backward();
      delay(150);
    }
    // Engel varsa ve çizgi yoksa ileri git
    else if ((leftSensor < THRESHOLD) && (rightSensor < THRESHOLD)) {
      forward();
    }
    else{
      stop(); // Aksi durumda dur
    }
  }
  else{
    // Engel yoksa çizgi kontrolü yap
    int leftSensor = analogRead(SensorLEFT);
    int rightSensor = analogRead(SensorRIGHT);
    delay(2);

    // Eğer çizgi algılandıysa geri git
    if ((leftSensor >= THRESHOLD) || (rightSensor >= THRESHOLD)) {
      backward();
      delay(150);
    }
    // Çizgi yoksa sola dön ve dur
    else if ((leftSensor < THRESHOLD) && (rightSensor < THRESHOLD)) {
      left();
      delay(50);
      stop();
    }
    else{
      stop(); // Belirsiz durumda dur
    }
  }
}
