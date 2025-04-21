from machine import Pin, PWM
import bluetooth
from rex import BLESimplePeripheral, Servo
import time

# Bluetooth Low Energy (BLE) nesnesi oluşturuluyor
ble = bluetooth.BLE()

# BLE nesnesi ile BLESimplePeripheral örneği oluşturuluyor
sp = BLESimplePeripheral(ble)

# Motor PWM kontrol pini
motor_pwm = Pin(13, Pin.OUT)

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

# Servo motorlar tanımlanıyor
Servo1 = Servo(2)
Servo2 = Servo(26)
Servo3 = Servo(18)
Servo4 = Servo(19)

# Buzzer pini
buzzer = Pin(25, Pin.OUT)

# Servo pozisyonları (başlangıç)
position1 = 90
position2 = 90
position3 = 90
position4 = 90

# Buzzer kontrol değişkenleri
playBuzzer = 0
buzzerStartTime = 0

# Varsayılan motor tipi (0 = DC motor)
Motor_Type = 0
DC_Motor = 0
Servo_Motor = 1

# Motor hızı (maksimum PWM değeri)
MotorSpeed = 65535

# Robotu ileri hareket ettiren fonksiyon
def forward(speed):
   motor_pwm.value(1)
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

# Bluetooth ile veri alındığında çağrılan fonksiyon
def on_rx(data):
    global buzzerStartTime, playBuzzer, Motor_Type, Servo_Motor, DC_Motor, position1, position2, position3, position4
    print("Veri alındı: ", data)

    # Motor tipi değiştirme (select ve start tuşları)
    last_mode = Motor_Type
    if data == b'\xff\x01\x01\x01\x02\x02\x00\x00': # Select → Servo
        Motor_Type = Servo_Motor
    elif data == b'\xff\x01\x01\x01\x02\x01\x00\x00': # Start → DC
        Motor_Type = DC_Motor
    else:
        Motor_Type = last_mode

    # Servo motor kontrolü
    if Motor_Type == Servo_Motor:
        if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': # Yukarı
            if position2 > 0:
                position2 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x02\x00': # Aşağı
            if position2 < 140:
                position2 += 5
        if data == b'\xff\x01\x01\x01\x02\x00\x04\x00': # Sol
            if position1 > 40:
                position1 -= 5
        if data == b'\xff\x01\x01\x01\x02\x00\x08\x00': # Sağ
            if position1 < 140:
                position1 += 5
        if data == b'\xff\x01\x01\x01\x02\x04\x00\x00': # Üçgen
            if position3 < 150:
                position3 += 5
        if data == b'\xff\x01\x01\x01\x02 \x00\x00': # Kare
            if position4 < 160:
                position4 += 5
        if data == b'\xff\x01\x01\x01\x02\x08\x00\x00': # Daire
            if position4 > 90:
                position4 -= 5
        if data == b'\xff\x01\x01\x01\x02\x10\x00\x00': # Çarpı
            if position3 > 30:
                position3 -= 5

        # Servo pozisyonlarını uygula
        Servo1.move(position1)
        Servo2.move(position2)
        Servo3.move(position3)
        Servo4.move(position4)

    # DC motor kontrolü
    else:
        if data == b'\xff\x01\x01\x01\x02\x00\x01\x00': # Yukarı
            forward(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x02\x00': # Aşağı
            backward(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x04\x00': # Sol
            left(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x00\x08\x00': # Sağ
            right(MotorSpeed)
        elif data == b'\xff\x01\x01\x01\x02\x04\x00\x00': # Üçgen
            print("üçgen tuşu")
        elif data == b'\xff\x01\x01\x01\x02 \x00\x00': # Kare
            print("kare tuşu")
        elif data == b'\xff\x01\x01\x01\x02\x08\x00\x00': # Daire
            print("daire tuşu")
        elif data == b'\xff\x01\x01\x01\x02\x10\x00\x00': # Çarpı → Buzzer çal
            buzzerStartTime = time.ticks_ms()
            playBuzzer = 1
            buzzer.value(1)
        else:
            stop()

# Ana döngü: bağlantı kontrolü ve buzzer zamanlaması
while True:
    currentTime = time.ticks_ms()
    
    # Buzzer'ı 1 saniye sonra kapat
    if (playBuzzer == 1) and (time.ticks_diff(currentTime, buzzerStartTime) > 1000):
         buzzer.value(0)
         playBuzzer = 0
         
    # Bluetooth bağlantısı varsa veri okuma fonksiyonunu aktif et
    if sp.is_connected():
        sp.on_write(on_rx)
