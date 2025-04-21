from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE nesnesi kullanılarak basit bir BLE sunucusu başlatılıyor
sp = BLESimplePeripheral(ble)

# Motor A pinleri tanımlanıyor ve başlangıçta durduruluyor
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# Motor B pinleri tanımlanıyor ve başlangıçta durduruluyor
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# Motor C pinleri tanımlanıyor ve başlangıçta durduruluyor
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# Motor D pinleri tanımlanıyor ve başlangıçta durduruluyor
motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Buzzer pini tanımlanıyor
buzzer = Pin(25, Pin.OUT)

# Buzzer kontrolü için değişkenler
playBuzzer = 0
buzzerStartTime = 0

# Motorların varsayılan hızı (maksimum PWM değeri)
MotorSpeed = 65535

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
   return

# Geri hareket fonksiyonu
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Sağa dönüş fonksiyonu
def right(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# Sola dönüş fonksiyonu
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

# Bluetooth üzerinden gelen veriyi işleyen geri çağırma fonksiyonu
def on_rx(data):
    global buzzerStartTime, playBuzzer
    print("Veri alındı: ", data)  # Alınan veri seri porta yazdırılıyor

    # Gelen verilere göre yönlendirme yapılır
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
    elif data == b'\xff\x01\x01\x01\x02 \x00\x00':     # kare
        print("square")
    elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00':  # daire
        print("circle")
    elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00':  # çarpı
        buzzerStartTime = time.ticks_ms()  # çarpıya basıldığında buzzer çal
        playBuzzer = 1
        buzzer.value(1)
    elif data == b'\xff\x01\x01\x01\x02\x02\x00\x00':  # select
        print("select")
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00':  # start
        print("start")
    else:
        stop()  # Bilinmeyen veri gelirse robot durur

# Ana döngü
while True:
    currentTime = time.ticks_ms()

    # Buzzer 1 saniye sonra otomatik kapanır
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    # Eğer BLE bağlantısı varsa gelen veriler dinlenir
    if sp.is_connected():
        sp.on_write(on_rx)  # Gelen veriler için callback tanımlanır
