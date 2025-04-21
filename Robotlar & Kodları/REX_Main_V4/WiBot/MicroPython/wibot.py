from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE bağlantısı ile BLESimplePeripheral nesnesi oluşturuluyor
sp = BLESimplePeripheral(ble)

# Motor A için PWM pinleri (ileri ve geri)
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

# Buzzer pini tanımlanıyor
buzzer = Pin(25, Pin.OUT)

# Buzzer kontrol değişkenleri
playBuzzer = 0
buzzerStartTime = 0

# Varsayılan motor hızı (maksimum hız)
MotorSpeed = 65535

# Robotu ileri hareket ettiren fonksiyon
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Robotu geri hareket ettiren fonksiyon
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Robotu sağa döndüren fonksiyon
def right(speed):
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
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# Tüm motorları durduran fonksiyon
def stop():
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# Bluetooth'tan veri alındığında çağrılan fonksiyon
def on_rx(data):
    global buzzerStartTime, playBuzzer
    print("Data received: ", data)  # Gelen veri yazdırılır
    
    # Her bir düğmenin tanımlı byte koduna göre robot kontrol edilir
    if data == b'\xff\x01\x01\x01\x02\x00\x01\x00':  # Yukarı tuşu
        forward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00':  # Aşağı tuşu
        backward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00':  # Sol tuş
        left(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00':  # Sağ tuş
        right(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00':  # Üçgen tuş
        print("trigle")
    elif data == b'\xff\x01\x01\x01\x02 \x00\x00':  # Kare tuş
        print("square")
    elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00':  # Daire tuş
        print("circle")
    elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00':  # Çarpı tuş
        buzzerStartTime = time.ticks_ms()
        playBuzzer = 1
        buzzer.value(1)  # Buzzer'ı aktif et
    elif data == b'\xff\x01\x01\x01\x02\x02\x00\x00':  # Select
        print("select")
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00':  # Start
        print("start")
    else:
        stop()  # Tanımsız komut geldiğinde durdur

# Ana döngü
while True:
    # Buzzer aktifse 1 saniye sonra kapat
    currentTime = time.ticks_ms()
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    # Bluetooth bağlantısı varsa, gelen veri için on_rx fonksiyonu atanır
    if sp.is_connected():
        sp.on_write(on_rx)
