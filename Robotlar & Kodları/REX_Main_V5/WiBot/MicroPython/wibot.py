from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE bağlantısı için basit bir haberleşme nesnesi oluşturuluyor
sp = BLESimplePeripheral(ble)

# Motorların enerjisini kontrol eden PWM pini tanımlanıyor
motor_pwm = Pin(13, Pin.OUT)

# Her motor için PWM kontrolü yapılacak pinler tanımlanıyor ve başlangıçta durduruluyor
# Motor A
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Buzzer pini (korna)
buzzer = Pin(25, Pin.OUT)

# Buzzer çalma kontrol değişkenleri
playBuzzer = 0
buzzerStartTime = 0

# Motorların varsayılan hızı
MotorSpeed = 65535

# Robotu ileri hareket ettiren fonksiyon
def forward(speed):
   motor_pwm.value(1)  # PWM sinyali aktif edilir

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
   motor_pwm.value(1)

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

# Robotu durduran fonksiyon
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

# Bluetooth üzerinden gelen veriyi işleyen fonksiyon
def on_rx(data):
    global buzzerStartTime, playBuzzer
    print("Veri alındı: ", data)

    # Düğmelere göre motor hareketleri
    if data == b'\xff\x01\x01\x01\x02\x00\x01\x00':  # yukarı ok
        forward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00':  # aşağı ok
        backward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00':  # sol ok
        left(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00':  # sağ ok
        right(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00':  # üçgen
        print("trigle")
    elif data == b'\xff\x01\x01\x01\x02 \x00\x00':  # kare
        print("square")
    elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00':  # daire
        print("circle")
    elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00':  # çarpı (buzzer çal)
        buzzerStartTime = time.ticks_ms()
        playBuzzer = 1
        buzzer.value(1)
    elif data == b'\xff\x01\x01\x01\x02\x02\x00\x00':  # select
        print("select")
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00':  # start
        print("start")
    else:
        stop()  # tanımsız veri geldiyse dur

# Ana döngü
while True:
    currentTime = time.ticks_ms()
    
    # Buzzer aktifse 1 saniye sonra otomatik kapat
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0

    # BLE bağlantısı varsa gelen veriyi işle
    if sp.is_connected():
        sp.on_write(on_rx)
