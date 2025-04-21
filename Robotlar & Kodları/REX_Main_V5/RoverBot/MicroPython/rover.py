from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE nesnesi ile BLESimplePeripheral sınıfından bir nesne oluşturuluyor
sp = BLESimplePeripheral(ble)

# Motorların güç kontrolü için PWM çıkışı tanımlanıyor
motor_pwm = Pin(13, Pin.OUT)

# Motor A için iki PWM pini (ileri ve geri yön)
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)  # Başlangıçta motor çalışmasın
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B için iki PWM pini
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C için iki PWM pini
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D için iki PWM pini
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Buzzer pin tanımı
buzzer = Pin(25, Pin.OUT)

# Buzzer çalma durumunu takip eden değişkenler
playBuzzer = 0
buzzerStartTime = 0

# Varsayılan motor hızı (maksimum PWM değeri)
MotorSpeed = 65535

# İleri hareket fonksiyonu
def forward(speed):
   motor_pwm.value(1)  # PWM pinini aktif et

   # Tüm motorları ileri yönde çalıştır
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)
   return

# Geri hareket fonksiyonu
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

# Sağ dönüş fonksiyonu
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

# Sol dönüş fonksiyonu
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

# Motorları durdurma fonksiyonu
def stop():
   motor_pwm.value(0)

   # Tüm motorları durdur
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# Gelen verileri işleyen callback fonksiyonu
def on_rx(data):
    global buzzerStartTime, playBuzzer
    print("Data received: ", data)  # Gelen veri konsola yazdırılır

    # Gelen veriye göre hareket fonksiyonları çağrılır
    if data == b'\xff\x01\x01\x01\x02\x00\x01\x00':  # Yukarı (ileri)
        forward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00':  # Aşağı (geri)
        backward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00':  # Sol
        left(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00':  # Sağ
        right(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00':  # Üçgen tuşu
        print("trigle")
    elif data == b'\xff\x01\x01\x01\x02 \x00\x00':  # Kare tuşu
        print("square")
    elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00':  # Daire tuşu
        print("circle")
    elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00':  # Çarpı tuşu
        buzzerStartTime = time.ticks_ms()
        playBuzzer = 1
        buzzer.value(1)
    elif data == b'\xff\x01\x01\x01\x02\x02\x00\x00':  # Select tuşu
        print("select")
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00':  # Start tuşu
        print("start")
    else:
        stop()  # Tanınmayan veri geldiğinde robotu durdur

# Ana döngü
while True:
    currentTime = time.ticks_ms()

    # Buzzer 1 saniye sonra kapanır
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0

    # BLE bağlantısı varsa gelen veriler alınır ve on_rx fonksiyonu tetiklenir
    if sp.is_connected():
        sp.on_write(on_rx)
