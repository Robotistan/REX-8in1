import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime
from rex import HCSR04  # Ultrasonik sensör için özel modül

# IR sensör pin tanımlamaları (analog girişler)
leftSensor = ADC(Pin(34))
rightSensor = ADC(Pin(35))

# Motor A pinleri (PWM)
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B pinleri (PWM)
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C pinleri (PWM)
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D pinleri (PWM)
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Motor hız değeri (maksimum 65535)
MotorSpeed = 50000

# Ultrasonik mesafe sensörü pinleri (trigger ve echo)
sensor = HCSR04(trigger_pin=17, echo_pin=16, echo_timeout_us=10000)

# IR sensör eşik değeri (bu değerin altı "siyah çizgi" olarak algılanır)
threshold = 65000

# İleri hareket fonksiyonu
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Sağa dönme fonksiyonu
def right(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Sola dönme fonksiyonu
def left(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Geri gitme fonksiyonu
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Tüm motorları durdurma fonksiyonu
def stop():
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
    distance = sensor.distance_cm()  # Ultrasonik sensör ile mesafe ölçümü (cm)
    
    # Eğer önünde engel varsa (15 cm'den yakınsa)
    if distance <= 15:
        leftSensorValue = leftSensor.read_u16()   # Sol IR sensör verisi
        rightSensorValue = rightSensor.read_u16() # Sağ IR sensör verisi
        sleep(0.02)
        
        # Eğer herhangi bir sensör çizgiyi algılamıyorsa (zemin beyaz), geri git
        if leftSensorValue >= threshold or rightSensorValue >= threshold:
            backward(MotorSpeed)
            sleep(0.5)
        # Her iki sensör çizgiyi algılıyorsa (zemin siyah), ileri git
        elif leftSensorValue < threshold and rightSensorValue < threshold:
            forward(MotorSpeed)
        else:
            stop()

    # Eğer önünde engel yoksa (15 cm'den uzaksa)
    else:
        leftSensorValue = leftSensor.read_u16()
        rightSensorValue = rightSensor.read_u16()
        sleep(0.02)

        # Herhangi biri çizgiyi algılamıyorsa, geri git
        if leftSensorValue >= threshold or rightSensorValue >= threshold:
            backward(MotorSpeed)
            sleep(0.5)
        # Her iki sensör çizgi üzerinde ise, sola dön
        elif leftSensorValue < threshold and rightSensorValue < threshold:
            left(MotorSpeed)
            sleep(0.1)
            stop()
        else:
            stop()
