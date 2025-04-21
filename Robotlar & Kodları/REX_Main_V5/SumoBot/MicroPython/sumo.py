import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime
from rex import HCSR04  # Ultrasonik sensör kütüphanesi

# IR sensör pinleri tanımlanıyor
leftSensor = ADC(Pin(34))   # Sol IR sensör analog pin
rightSensor = ADC(Pin(35))  # Sağ IR sensör analog pin

# Motorları çalıştırmak için PWM kontrol pini
motor_pwm = Pin(13, Pin.OUT)

# Motor A kontrol pinleri (sağ ön teker)
motor_A1 = PWM(Pin(15))  # İleri yön
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))  # Geri yön
motor_A2.duty_u16(0)

# Motor B kontrol pinleri (sol ön teker)
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C kontrol pinleri (sol arka teker)
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D kontrol pinleri (sağ arka teker)
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Motorlara verilecek varsayılan hız değeri (16-bit)
MotorSpeed = 50000

# Ultrasonik sensör başlatılıyor
sensor = HCSR04(trigger_pin=17, echo_pin=16, echo_timeout_us=10000)

# IR sensörlerden gelen değerin eşiği
threshold = 65000

# Robotu ileri hareket ettiren fonksiyon
def forward(speed):
   motor_pwm.value(1)  # PWM sinyalini aç

   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Robotu sağa döndüren fonksiyon
def right(speed):
   motor_pwm.value(1)

   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Robotu sola döndüren fonksiyon
def left(speed):
   motor_pwm.value(1)

   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Robotu geri hareket ettiren fonksiyon
def backward(speed):
   motor_pwm.value(1)

   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Robotu tamamen durduran fonksiyon
def stop():
   motor_pwm.value(0)

   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# Ana döngü
while True:
    distance = sensor.distance_cm()  # Ultrasonik sensörle mesafe ölç
    # print(distance)  # Mesafe terminale yazdırılabilir

    if distance <= 15:
        # Engel 15 cm'den yakınsa:
        leftSensorValue = leftSensor.read_u16()
        rightSensorValue = rightSensor.read_u16()
        sleep(0.02)

        # Eğer çizgiye çok yakınsa geri çekil
        if leftSensorValue >= threshold or rightSensorValue >= threshold:
            backward(MotorSpeed)
            sleep(0.5)

        # Çizgi yoksa ileri git
        elif leftSensorValue < threshold and rightSensorValue < threshold:
            forward(MotorSpeed)

        # Belirsiz durumdaysa dur
        else:
            stop()

    else:
        # Engel uzaksa çizgi sensörlerini kontrol et
        leftSensorValue = leftSensor.read_u16()
        rightSensorValue = rightSensor.read_u16()
        sleep(0.02)

        if leftSensorValue >= threshold or rightSensorValue >= threshold:
            backward(MotorSpeed)
            sleep(0.5)

        elif leftSensorValue < threshold and rightSensorValue < threshold:
            left(MotorSpeed)  # Sola dön
            sleep(0.1)
            stop()

        else:
            stop()
