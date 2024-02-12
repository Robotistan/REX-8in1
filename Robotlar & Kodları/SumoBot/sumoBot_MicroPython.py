import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime
from rex import HCSR04

# IR Line Tracker (çizgi izleyen) sensörün pinlerini tanımlayalım.  
leftSensor = ADC(Pin(34))
rightSensor = ADC(Pin(35))

#motorA
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

#motorB
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

#motorC
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

#motorD
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

#speed values
MotorSpeed = 50000

#HCSR04 (Mesafe sensörü) pinlerini tanımlayalım.
sensor = HCSR04(trigger_pin=17, echo_pin=16, echo_timeout_us=10000)

# IR Line Tracker (çizgi izleyen) sensörün algılayaca sınır değer.
threshold = 65000

# İleri (forward) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)
   
# Sağ (right) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def right(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)
   
# Sol (left) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def left(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Geri (backward) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)
   
# Robotun durması için gerekli motor hareketlerini "stop" adlı fonksiyona tanımlayalım. 
def stop():
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

while True:
    # mesafe değerini ölçelim ve cm değerinde algılanan veriyi değişkene atayalım.
    distance = sensor.distance_cm()
    
    #print(distance)
    if distance <= 15: # mesafe değeri 15'den küçük ya da eşitse
        # IR Line Tracker Sensörün değerlerini değişkenlere ata
        leftSensorValue = leftSensor.read_u16()
        rightSensorValue = rightSensor.read_u16()
        #print(leftSensorValue)
        #print(rightSensorValue)
        sleep(0.02)
        # Eğer sensör değerleri belirlenen sınır (threshold) değerinden büyük ya da eşitse
        if leftSensorValue >= threshold or rightSensorValue >= threshold: 
            backward(MotorSpeed) #geri git
            sleep(0.5) # 500 ms bekle
        # Eğer sensör değerleri belirlenen sınır (threshold) değerinden küçük ya da eşitse
        elif leftSensorValue < threshold and rightSensorValue < threshold:
            forward(MotorSpeed) #ileri git
        else:
            stop() #dur
    else:
        # IR Line Tracker Sensörün değerlerini değişkenlere ata
        leftSensorValue = leftSensor.read_u16()
        rightSensorValue = rightSensor.read_u16()
        #print(leftSensorValue)
        #print(rightSensorValue)
        sleep(0.02)
        # Eğer sensör değerleri belirlenen sınır (threshold) değerinden büyük ya da eşitse
        if leftSensorValue >= threshold or rightSensorValue >= threshold:
            backward(MotorSpeed) # geri git
            sleep(0.5) # 500 ms bekle
        # Eğer sensör değerleri belirlenen sınır (threshold) değerinden küçük ya da eşitse
        elif leftSensorValue < threshold and rightSensorValue < threshold:
            left(MotorSpeed) #sola dön
            sleep(0.1) # 100 ms git
            stop() #dur
        else:
            stop() #dur


