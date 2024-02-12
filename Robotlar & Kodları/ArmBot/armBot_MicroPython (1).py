from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral, Servo
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

#servo
Servo1 = Servo(2)
Servo2 = Servo(26)
Servo3 = Servo(18)
Servo4 = Servo(19)

#buzzer
buzzer = Pin(25, Pin.OUT)

position1 = 90;
position2 = 90;
position3 = 90;
position4 = 90;

playBuzzer = 0
buzzerStartTime = 0
Motor_Type = 0

# Motor tipleri (varsayılan 0) 
DC_Motor = 0
Servo_Motor = 1

# varsayılan motor hızı (%100)
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


# ArmBot için BLE uygulamadan alınan verileri işlemek için "on_rx" fonksiyonunu tanımlayalım.  
def on_rx(data):
    global buzzerStartTime, playBuzzer, Motor_Type, Servo_Motor, DC_Motor, position1, position2, position3, position4
    print("Data received: ", data)  # Gelen veriyi yazdıralım.
    
    last_mode = Motor_Type
    if data == b'\xff\x01\x01\x01\x02\x02\x00\x00': #select butonuna basıldığında.
        Motor_Type = Servo_Motor
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00': #start butonuna basıldığında. 
        Motor_Type = DC_Motor
    else:
        Motor_Type = last_mode

    if Motor_Type == Servo_Motor:
        if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': #up (yukarı) butonuna basıldığında.
            if position2 > 0:
                position2 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x02\x00': #down (aşağı) butonuna basıldığında.
            if (position2 < 140):
                position2 += 5
        if data == b'\xff\x01\x01\x01\x02\x00\x04\x00': #left (sol) butonuna basıldığında.
            if (position1 > 40):
                position1 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x08\x00': #right (sağ) butonuna basıldığında.
            if (position1 < 140):
                position1 += 5
        if data == b'\xff\x01\x01\x01\x02\x04\x00\x00': #trigle (üçgen) butonuna basıldığında.
            if (position3 < 150):
                position3 += 5
        if data == b'\xff\x01\x01\x01\x02 \x00\x00': #square (kare) butonuna basıldığında.
            if (position4 < 160):
                position4 += 5
        if data == b'\xff\x01\x01\x01\x02\x08\x00\x00': #circle (daire) butonuna basıldığında.
            if (position4 > 90):
                position4 -= 5
        if data == b'\xff\x01\x01\x01\x02\x10\x00\x00': #cross (çarpı) (X) butonuna basıldığında.
            if (position3 > 30):
                position3 -= 5
        # BLE den gelen veriye göre servo motorları güncel pozisyonlara hareket ettir.
        Servo1.move(position1)
        Servo2.move(position2)
        Servo3.move(position3)
        Servo4.move(position4)
    else:
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
        else:
            stop()
while True:
    currentTime = time.ticks_ms()
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    if sp.is_connected():  # BLE bağlantısının bağlanıp bağlanmadığını kontrol edin.
        sp.on_write(on_rx)  # Verileri almak için gerekli olan fonksiyonları çağırın. 


