# Gerekli kütüphaneler
import machine
from machine import Pin, PWM
from rex import ESP32_BLE, Servo
import time

# Motor pin tanımlamaları (her motorun ileri ve geri kontrolü için ayrı pin)
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

motor_D1 = PWM(Pin(27))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(14))
motor_D2.duty_u16(0)

# Servo motor pin tanımlamaları
Servo1 = Servo(2)   # Servo 1: İleri-Geri
Servo2 = Servo(26)  # Servo 2: Sağ-Sol
Servo3 = Servo(18)  # Servo 3: Yukarı-Aşağı
Servo4 = Servo(19)  # Servo 4: Aç-Kapa

# Buzzer tanımı
buzzer_pin = Pin(25, Pin.OUT)
buzzer_pwm = machine.PWM(buzzer_pin, freq=1000, duty=0)

# Servo motorların varsayılan açıları
position1 = 90
position2 = 90
position3 = 90
position4 = 90

# Servo motorları başlangıç pozisyonuna getir
Servo1.move(position1)
Servo2.move(position2)
Servo3.move(position3)
Servo4.move(position4)

# Hareket yönleri
STOP = 0
FWD = 1
BWD = 2
RIGHT = 3
LEFT = 4
FWD_RIGHT = 5
FWD_LEFT = 6
BWD_RIGHT = 7
BWD_LEFT = 8

# Piyano notalarının frekansları
NOTE_C  = 262
NOTE_CS = 278
NOTE_D  = 294
NOTE_DS = 312
NOTE_E  = 330
NOTE_F  = 350
NOTE_FS = 370
NOTE_G  = 392
NOTE_GS = 416
NOTE_A  = 440
NOTE_AS = 467
NOTE_B  = 494
NOTE_C2 = 524

# Kontrol modları
joystick = 1
buttons = 2

# Joystick merkez ve ölü bölge ayarları
deadZone = 20
centerX = 127
centerY = 127

# Değerleri bir aralıkta kısıtlayan fonksiyon
def constrain(val, min_val, max_val):
    return max(min_val, min(max_val, val))

# Değeri farklı bir aralığa dönüştüren fonksiyon
def custom_map(value, in_min, in_max, out_min, out_max):
    return (value - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

# Tekerlekli robot hareket fonksiyonu (Rover, WiBot, ArmBot)
def move(direction, speed):
    avg = constrain(speed, 150, 255)
    duty = custom_map(avg, 150, 255, 50000, 65535)

    # Belirtilen yöne göre motorlara hız ver
    if direction == FWD:
        motor_A1.duty_u16(duty)
        motor_A2.duty_u16(0)
        motor_B1.duty_u16(duty)
        motor_B2.duty_u16(0)
        motor_C1.duty_u16(duty)
        motor_C2.duty_u16(0)
        motor_D1.duty_u16(duty)
        motor_D2.duty_u16(0)
    # ... Diğer yönler benzer şekilde tanımlanmış ...

    else:  # STOP
        motor_A1.duty_u16(0)
        motor_A2.duty_u16(0)
        motor_B1.duty_u16(0)
        motor_B2.duty_u16(0)
        motor_C1.duty_u16(0)
        motor_C2.duty_u16(0)
        motor_D1.duty_u16(0)
        motor_D2.duty_u16(0)

# Omni yönlü robotlar için hareket fonksiyonu
def omni_move(direction, speed):
    avg = constrain(speed, 150, 255)
    duty = custom_map(avg, 150, 255, 50000, 65535)

    # Belirtilen yöne göre motorlara hız ver (omni hareket mantığı)
    # ... Aynı mantıkla omni yönleri tanımlanıyor ...

    else:  # STOP
        motor_A1.duty_u16(0)
        motor_A2.duty_u16(0)
        motor_B1.duty_u16(0)
        motor_B2.duty_u16(0)
        motor_C1.duty_u16(0)
        motor_C2.duty_u16(0)
        motor_D1.duty_u16(0)
        motor_D2.duty_u16(0)

# Buzzer'ı kısa süreliğine öttürür
def rex_horn():
    buzzer_pwm.duty(256)
    buzzer_pwm.freq(262)
    time.sleep(0.2)
    buzzer_pwm.duty(0)

# Belirli bir notayı çalmak için kullanılır
def piano_tone(note):
    buzzer_pwm.duty(256)
    buzzer_pwm.freq(note)
    time.sleep(0.2)
    buzzer_pwm.duty(0)

# BLE cihazı başlatılır
ble_device = ESP32_BLE()

# Ana döngü
while True:
    buffer = ble_device.get_data()
    if buffer:
        if buffer[0] == 75:  # Başlangıç baytı: 75
            # buffer[1] robot türü, buffer[2] komut türü
            # ... Yukarıda detaylandırılmış ...
            
            # Servo motor kontrolü örneği:
            if buffer[1] == 3:  # ArmBot
                if buffer[2] == 1:
                    # Tüm servo motorları sıfırla
                    position1 = 90
                    Servo1.move(position1)
                    # ... diğerlerini de sıfırla ...

                elif buffer[2] == 4:  # Servo kontrol
                    if buffer[3] == 1:  # Servo1 sola
                        if position1 > 40:
                            position1 -= 5
                            Servo1.move(position1)

        # Veriyi temizle
        ble_device.clear_data()
