from machine import Pin, ADC, PWM
import time
from rex import HCSR04

# Motorları çalıştırmak için PWM sinyali kontrol pini
motor_pwm = Pin(13, Pin.OUT)

# Motor A'nın kontrol pinleri
motor_A1 = PWM(Pin(15))  # İleri yönde çalıştırma
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))  # Geri yönde çalıştırma
motor_A2.duty_u16(0)

# Motor B'nin kontrol pinleri
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C'nin kontrol pinleri
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D'nin kontrol pinleri
motor_D1 = PWM(Pin(14))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(27))
motor_D2.duty_u16(0)

# Buzzer (korna) pini
buzzer = Pin(25, Pin.OUT)

# Ultrasonik mesafe sensörü tanımı
sensor = HCSR04(trigger_pin=17, echo_pin=16, echo_timeout_us=10000)

# Motorların varsayılan hızı
MotorSpeed = 50000

# Robotu ileri hareket ettiren fonksiyon
def forward(speed):
   motor_pwm.value(1)  # PWM sinyalini aktif et
   
   # Tüm motorları ileri yönde döndür
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Robotu tamamen durduran fonksiyon
def stop():
   motor_pwm.value(0)  # PWM sinyalini kapat

   # Tüm motorlara sıfır sinyal gönder
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# Robotu geri yönde hareket ettiren fonksiyon
def backward(speed):
   motor_pwm.value(1)
   
   # Tüm motorları geri yönde döndür
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Robotu sola döndüren fonksiyon
def left(speed):
   motor_pwm.value(1)
   
   # Sol taraf motorları geri, sağ taraf ileri döner
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Ana döngü - robotun davranışlarını kontrol eder
while True:
    distance = sensor.distance_cm()  # Mesafe ölç
    print(distance)  # Ölçülen mesafeyi yazdır
    time.sleep(0.5)  # 0.5 saniye bekle
    # Eğer engel 12 cm'den uzaktaysa ileri git
    if distance > 12:
        forward(MotorSpeed)
    else:
        # Engel yakınsa geri git, buzzer çal, sola dön ve dur
        backward(MotorSpeed)
        buzzer.value(1)
        time.sleep(0.3)
        buzzer.value(0)
        left(MotorSpeed)
        time.sleep(0.2)
        stop()
