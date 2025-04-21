from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral, Servo
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE üzerinden iletişim sağlayacak nesne oluşturuluyor
sp = BLESimplePeripheral(ble)

# Motor A pin tanımlamaları ve PWM başlatma
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B pin tanımlamaları
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C pin tanımlamaları
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D pin tanımlamaları
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Servo motor nesneleri tanımlanıyor
Servo1 = Servo(2)
Servo2 = Servo(26)
Servo3 = Servo(18)
Servo4 = Servo(19)

# Buzzer pin tanımlaması
buzzer = Pin(25, Pin.OUT)

# Servo başlangıç pozisyonları
position1 = 90
position2 = 90
position3 = 90
position4 = 90

# Buzzer kontrol değişkenleri
playBuzzer = 0
buzzerStartTime = 0

# Motor tipi kontrolü için değişken (0 = DC, 1 = Servo)
Motor_Type = 0
DC_Motor = 0
Servo_Motor = 1

# Varsayılan motor hızı (maksimum PWM değeri)
MotorSpeed = 65535

# DC motorları ileri yönde döndürür
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Geri yönde hareket
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Sağa dönüş
def right(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Sola dönüş
def left(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Tüm motorları durdur
def stop():
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# BLE üzerinden veri alındığında çalışacak fonksiyon
def on_rx(data):
    global buzzerStartTime, playBuzzer, Motor_Type, Servo_Motor, DC_Motor
    global position1, position2, position3, position4

    print("Gelen veri: ", data)

    # En son hangi moddaydık (servo ya da DC), tekrar kullanmak için kaydedilir
    last_mode = Motor_Type

    # Select tuşu => servo motor moduna geç
    if data == b'\xff\x01\x01\x01\x02\x02\x00\x00':
        Motor_Type = Servo_Motor
    # Start tuşu => DC motor moduna geç
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00':
        Motor_Type = DC_Motor
    else:
        Motor_Type = last_mode

    # Eğer servo modundaysak joystick hareketlerine göre açıları değiştir
    if Motor_Type == Servo_Motor:
        if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': # yukarı
            if position2 > 0:
                position2 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x02\x00': # aşağı
            if position2 < 140:
                position2 += 5
        if data == b'\xff\x01\x01\x01\x02\x00\x04\x00': # sol
            if position1 > 40:
                position1 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x08\x00': # sağ
            if position1 < 140:
                position1 += 5
        if data == b'\xff\x01\x01\x01\x02\x04\x00\x00': # üçgen
            if position3 < 150:
                position3 += 5
        if data == b'\xff\x01\x01\x01\x02 \x00\x00': # kare
            if position4 < 160:
                position4 += 5
        if data == b'\xff\x01\x01\x01\x02\x08\x00\x00': # daire
            if position4 > 90:
                position4 -= 5
        if data == b'\xff\x01\x01\x01\x02\x10\x00\x00': # çarpı
            if position3 > 30:
                position3 -= 5

        # Güncel açı değerlerini servo motorlara gönder
        Servo1.move(position1)
        Servo2.move(position2)
        Servo3.move(position3)
        Servo4.move(position4)

    # DC motor modundaysak yönlere göre hareket ettir
    else:
        if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': # yukarı
            forward(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00': # aşağı
            backward(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00': # sol
            left(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00': # sağ
            right(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00': # üçgen
            print("trigle")
        elif data == b'\xff\x01\x01\x01\x02 \x00\x00': # kare
            print("square")
        elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00': # daire
            print("circle")
        elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00': # çarpı
            buzzerStartTime = time.ticks_ms()
            playBuzzer = 1
            buzzer.value(1)
        else:
            stop()

# Ana döngü
while True:
    currentTime = time.ticks_ms()
    
    # Buzzer 1 saniye boyunca çalışsın
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    # BLE bağlantısı aktifse, veri alımı için dinle
    if sp.is_connected():
        sp.on_write(on_rx)  # Veri gelirse on_rx fonksiyonunu çağır
