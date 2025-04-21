//"""REX 8in1 Balance Bot"""
// ESP32 kart sürümü 2.0.11 olmalıdır

#include "I2Cdev.h"                     // I2C cihaz haberleşme kütüphanesi
#include "PID_v1.h"                     // PID kontrol kütüphanesi
#include "MPU6050_6Axis_MotionApps20.h"// MPU6050 DMP (Motion Processing) sürücüsü
#include <Wire.h>                      // I2C haberleşme kütüphanesi

#define INTERRUPT_PIN 13               // MPU6050'nin interrupt pini tanımı

// Motor pin tanımlamaları (A ve C motorları kullanılıyor)
#define Motor_A1 15
#define Motor_A2 23
#define Motor_C1 5
#define Motor_C2 4

MPU6050 mpu;                           // MPU6050 sensör nesnesi

bool dmpReady = false;                // DMP başarıyla başlatıldığında true olur

uint8_t mpuIntStatus;                 // MPU'dan gelen kesme (interrupt) durum bayrağı
uint8_t devStatus;                    // Cihaz başlangıç durumu (0 = başarılı, !0 = hata)
uint16_t packetSize;                 // DMP veri paketi boyutu
uint16_t fifoCount;                  // FIFO içindeki bayt sayısı
uint8_t fifoBuffer[64];              // FIFO'dan alınan veriler burada saklanır

// Yönelim değişkenleri
Quaternion q;                        // [w, x, y, z] quaternion
VectorFloat gravity;                 // Yerçekimi vektörü
float ypr[3];                        // [yaw, pitch, roll]

// BOT'un denge konumu için PID parametreleri
double setpoint = 189.7;             // BOT yere dik olduğunda okunan değer
double Kp = 4;                       // Oransal kazanç
double Kd = 0.2;                     // Türevsel kazanç
double Ki = 40;                      // İntegral kazanç

double input, output;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT); // PID nesnesi

bool interrupteddd = false;
volatile bool mpuInterrupt = false;  // MPU interrupt bayrağı
void dmpDataReady()
{
  mpuInterrupt = true;
  interrupteddd = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();  // I2C başlatılıyor

  Serial.println(F("I2C aygıtları başlatılıyor..."));
  mpu.initialize(); // MPU başlatılıyor

  Serial.println(F("Cihaz bağlantısı test ediliyor..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 bağlantı başarılı") : F("MPU6050 bağlantı başarısız"));

  devStatus = mpu.dmpInitialize(); // DMP başlatılıyor

  // Kalibrasyon (her cihaza göre değişebilir)
  mpu.setXGyroOffset(74);
  mpu.setYGyroOffset(12);
  mpu.setZGyroOffset(5);
  mpu.setZAccelOffset(1094);

  if (devStatus == 0)
  {
    Serial.println(F("DMP etkinleştiriliyor..."));
    mpu.setDMPEnabled(true);

    pinMode(INTERRUPT_PIN, INPUT);
    Serial.println(F("Interrupt algılaması başlatılıyor..."));
    attachInterrupt(INTERRUPT_PIN, dmpDataReady, RISING); // Harici kesme tanımlanıyor
    mpuIntStatus = mpu.getIntStatus();

    Serial.println(F("DMP hazır! İlk interrupt bekleniyor..."));
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize(); // Beklenen DMP paket boyutu

    // PID yapılandırması
    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(10);                // 10 ms örnekleme zamanı
    pid.SetOutputLimits(-200, 200);       // Çıkış sınırları
  }
  else
  {
    Serial.print(F("DMP başlatma hatası (kod "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  // Motor pinleri çıkış olarak ayarlanıyor
  pinMode(Motor_A1, OUTPUT);
  pinMode(Motor_A2, OUTPUT);
  pinMode(Motor_C1, OUTPUT);
  pinMode(Motor_C2, OUTPUT);

  // Motorlar başlangıçta kapalı
  analogWrite(Motor_A1, LOW);
  analogWrite(Motor_A2, LOW);
  analogWrite(Motor_C1, LOW);
  analogWrite(Motor_C2, LOW);
}

void loop() {
  if (!dmpReady) return; // DMP hazır değilse çalışmaz

  // DMP verisi gelene kadar beklenir
  while (!mpuInterrupt && fifoCount < packetSize)
  {
    // Veriler yokken PID işlemleri yapılır
    pid.Compute();

    // Eğer bot dengede değilse motorlar hareket ettirilir
    if (input > 120 && input < 230) {
      if (output > 0)        // Öne düşüyorsa
        Forward();           // İleri hareket
      else if (output < 0)   // Arkaya düşüyorsa
        Reverse();           // Geri hareket
    }
    else
      Stop();                // Dengedeyse motorları durdur
  }

  // Kesme bayrağı sıfırlanır ve yeni veri alınır
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  // FIFO taşma kontrolü
  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    mpu.resetFIFO(); // Taşma olursa FIFO sıfırlanır
    Serial.println(F("FIFO overflow!"));
  }
  else if (mpuIntStatus & 0x02)
  {
    // Paket boyutu kadar veri beklenir
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // FIFO'dan veri alınır
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;

    // Yönelim verileri hesaplanır
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // PID girişi olarak roll değeri alınır (derece cinsine çevrilir)
    input = ypr[1] * 180 / M_PI + 180;
  }
}

void Forward() // Motorları ileri döndür
{
  analogWrite(Motor_A1, output + 55);
  analogWrite(Motor_A2, 0);
  analogWrite(Motor_C1, output + 55);
  analogWrite(Motor_C2, 0);
}

void Reverse() // Motorları geri döndür
{
  analogWrite(Motor_A1, 0);
  analogWrite(Motor_A2, (output * -1) + 55);
  analogWrite(Motor_C1, 0);
  analogWrite(Motor_C2, (output * -1) + 55);
}

void Stop() // Motorları durdur
{
  analogWrite(Motor_A1, 0);
  analogWrite(Motor_A2, 0);
  analogWrite(Motor_C1, 0);
  analogWrite(Motor_C2, 0);
}
