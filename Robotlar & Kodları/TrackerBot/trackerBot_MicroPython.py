import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime

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

#hız değerleri
Mid_Speed = 50000
Low_Speed = 47000

STOP = 0
FWD = 1
LEFT = 2
RIGHT = 3
BWD = 4

# IR Line Tracker (çizgi izleyen) sensörün algılayaca sınır değer.
threshold = 65000

directionStt = STOP
oldDirection = STOP

#İleri (forward) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

   return

#Sağ (right) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def right(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)
   return

#Sol (left) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def left(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)
   return

#Geri (geri) yönü için gerekli motor hareketlerini fonksiyona tanımlayalım. 
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

   return

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
    # IR Line Tracker Sensörün değerlerini değişkenlere ata
    leftSensorValue = leftSensor.read_u16()
    rightSensorValue = rightSensor.read_u16()
    #print(leftSensorValue)
    #print(rightSensorValue)
    sleep(0.02)
    
    # IR Line Tracker Sensörün değerlerine göre robotun hareketlerine karar veren koşul yapısı
    if leftSensorValue >= threshold and rightSensorValue >= threshold:
        directionStt = FWD
    elif leftSensorValue < threshold and rightSensorValue > threshold:
        directionStt = RIGHT
    elif leftSensorValue > threshold and rightSensorValue < threshold:
        directionStt = LEFT
    elif leftSensorValue < threshold and rightSensorValue < threshold and directionStt != STOP:
        directionStt = BWD
        
    # belirlenen hareket fonksiyonlarını çalıştıran koşul yapısı     
    if directionStt != oldDirection:
        oldDirection = directionStt
        if directionStt == FWD:
            forward(Mid_Speed)
        elif directionStt == RIGHT:
            right(Mid_Speed)
        elif directionStt == LEFT:
            left(Mid_Speed)
        elif directionStt == BWD:
            backward(Low_Speed)
            reversTime = utime.ticks_ms()
        elif directionStt == STOP:
            stop()
            
        #print(directionStt)
        #print(oldDirection)
    if directionStt == ("BWD") and (utime.ticks_ms()) - reversTime > (300):
        directionStt = "STOP"