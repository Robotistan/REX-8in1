from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral
import time

# Bluetooth Low Energy (BLE) nesnesini bluetooth kütüphanesininden çağıralım.
# Oluşturduğumuz değişkenlere atayalım. 
ble = bluetooth.BLE()
sp = BLESimplePeripheral(ble)

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

#buzzer
buzzer = Pin(25, Pin.OUT)

playBuzzer = 0
buzzerStartTime = 0

#varsayılan motor hızı (%100)
MotorSpeed = 65535

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
   return

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

# RoverBot'un durması için gerekli motor hareketlerini "stop" fonksiyonuna tanımlayalım. 
def stop():
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)

   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)

   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)

   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# RoverBot için BLE uygulamadan alınan verileri işlemek için "on_rx" fonksiyonunu tanımlayalım. 
def on_rx(data):
    global buzzerStartTime, playBuzzer
    print("Data received: ", data)  # Gelen veriyi yazdıralım.
    
    if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': #up (yukarı) butonuna basıldığında.
        forward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00': #down (aşağı) butonuna basıldığında.
        backward(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00': #left (sol) butonuna basıldığında.
        left(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00': #right (sağ) butonuna basıldığında.
        right(MotorSpeed)
    elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00': #trigle (üçgen) butonuna basıldığında.
        print("trigle")
    elif data == b'\xff\x01\x01\x01\x02 \x00\x00': #square (kare) butonuna basıldığında.
        print("square")
    elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00': #circle (daire) butonuna basıldığında.
        print("circle")
    elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00': #cross (çarpı) (X) butonuna basıldığında.
        buzzerStartTime = time.ticks_ms()
        playBuzzer = 1
        buzzer.value(1)
    elif data == b'\xff\x01\x01\x01\x02\x02\x00\x00': #select butonuna basıldığında.
        print("select")
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00': #start butonuna basıldığında.
        print("start")
    else:
        stop()

while True:
    currentTime = time.ticks_ms()
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    if sp.is_connected():  # BLE bağlantısının bağlanıp bağlanmadığını kontrol edin.
        sp.on_write(on_rx)  # Verileri almak için gerekli olan fonksiyonları çağırın. 


