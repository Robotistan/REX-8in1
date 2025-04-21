import machine
from machine import Pin, ADC, PWM, Timer
from time import sleep
import utime

# IR sensör pinleri (analog)
leftSensor = ADC(Pin(34))    # Sol sensör
rightSensor = ADC(Pin(35))   # Sağ sensör

# Motor A pinleri
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B pinleri
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C pinleri
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D pinleri
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Motor hızları
Mid_Speed = 50000
Low_Speed = 47000

# Hareket yönü sabitleri
STOP = 0     # Dur
FWD = 1      # İleri
LEFT = 2     # Sola dön
RIGHT = 3    # Sağa dön
BWD = 4      # Geri

# Sensör eşiği (çizgiyi algılamak için)
threshold = 65000

# Yön takip değişkenleri
directionStt = STOP
oldDirection = STOP

# İleri gitme fonksiyonu
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
    return

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
    return

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
    return

# Durma fonksiyonu
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
    # IR sensörlerden veri oku
    leftSensorValue = leftSensor.read_u16()
    rightSensorValue = rightSensor.read_u16()

    sleep(0.02)  # Okuma stabilitesi için kısa bekleme

    # Her iki sensör de çizgiyi görüyorsa ileri git
    if leftSensorValue >= threshold and rightSensorValue >= threshold:
        directionStt = FWD
    # Sadece sağ sensör çizgiyi görüyorsa sağa dön
    elif leftSensorValue < threshold and rightSensorValue > threshold:
        directionStt = RIGHT
    # Sadece sol sensör çizgiyi görüyorsa sola dön
    elif leftSensorValue > threshold and rightSensorValue < threshold:
        directionStt = LEFT
    # Hiçbiri görmüyorsa ve daha önce durmuyorsa geri git
    elif leftSensorValue < threshold and rightSensorValue < threshold and directionStt != STOP:
        directionStt = BWD

    # Yön değiştiyse ilgili fonksiyonu çağır
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

    # Geri gitme süresi 300ms'yi geçtiyse dur
    if directionStt == BWD and (utime.ticks_ms() - reversTime > 300):
        directionStt = STOP
