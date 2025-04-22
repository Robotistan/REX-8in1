import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime

# IR sensör pinleri tanımlanıyor (analog giriş)
leftSensor = ADC(Pin(34))     # Sol IR sensör
rightSensor = ADC(Pin(35))    # Sağ IR sensör

# Motorları kontrol etmek için PWM çıkışı (motor güç anahtarı gibi)
motor_pwm = Pin(13, Pin.OUT)

# Motor A için PWM pinleri
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B için PWM pinleri
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C için PWM pinleri
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D için PWM pinleri
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Hız değerleri
Mid_Speed = 50000     # Normal hız
Low_Speed = 47000     # Düşük hız (geri giderken kullanılır)

# Yön sabitleri
STOP = 0
FWD = 1
LEFT = 2
RIGHT = 3
BWD = 4

# IR sensör eşiği
threshold = 65000

# Yön durum değişkenleri
directionStt = STOP
oldDirection = STOP

# İleri gitme fonksiyonu
def forward(speed):
   motor_pwm.value(1)  # PWM pinini aktif et

   # Tüm motorları ileri çalıştır
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)
   return

# Sağa dönme fonksiyonu
def right(speed):
   motor_pwm.value(1)

   # Sol tekerlekler ileri, sağ tekerlekler geri
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)
   return

# Sola dönme fonksiyonu
def left(speed):
   motor_pwm.value(1)

   # Sağ tekerlekler ileri, sol tekerlekler geri
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)
   return

# Geri gitme fonksiyonu
def backward(speed):
   motor_pwm.value(1)

   # Tüm motorlar geri çalışır
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)
   return

# Robotu durdurma fonksiyonu
def stop():
   motor_pwm.value(0)

   # Tüm motorlara 0 sinyali gönder
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
    # IR sensör değerlerini oku
    leftSensorValue = leftSensor.read_u16()
    rightSensorValue = rightSensor.read_u16()

    # Kısa bekleme
    sleep(0.02)

    # Yön belirleme mantığı
    if leftSensorValue >= threshold and rightSensorValue >= threshold:
        directionStt = FWD  # İleri git
    elif leftSensorValue < threshold and rightSensorValue > threshold:
        directionStt = RIGHT  # Sağa dön
    elif leftSensorValue > threshold and rightSensorValue < threshold:
        directionStt = LEFT   # Sola dön
    elif leftSensorValue < threshold and rightSensorValue < threshold and directionStt != STOP:
        directionStt = BWD    # Çizgiyi kaybetti, geri git

    # Eğer yön değiştiyse, harekete geç
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
            reversTime = utime.ticks_ms()  # Geri gitmeye başlama zamanı
        elif directionStt == STOP:
            stop()

    # Geri gitme süresi 300ms'den fazlaysa dur
    if directionStt == ("BWD") and (utime.ticks_ms()) - reversTime > (300):
        directionStt = "STOP"
