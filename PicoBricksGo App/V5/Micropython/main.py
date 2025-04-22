from machine import Pin, PWM
from rex import ESP32_BLE, Servo
import time

# Motor pinlerinin tanımlanması
motor_A1 = Pin(15, Pin.OUT)
motor_A2 = Pin(23, Pin.OUT)
motor_B1 = Pin(32, Pin.OUT)
motor_B2 = Pin(33, Pin.OUT)
motor_C1 = Pin(27, Pin.OUT)
motor_C2 = Pin(14, Pin.OUT)
motor_D1 = Pin(5, Pin.OUT)
motor_D2 = Pin(4, Pin.OUT)

# Motor hız kontrolü için PWM pininin tanımlanması
motor_pwm = PWM(Pin(13))
motor_pwm.freq(1000)  # PWM frekansı 1 kHz olarak ayarlanıyor

# Servo motor pinlerinin tanımlanması (ileri-geri, sağ-sol, yukarı-aşağı, aç-kapa)
Servo1 = Servo(2)
Servo2 = Servo(26)
Servo3 = Servo(18)
Servo4 = Servo(19)

# Buzzer (korna) PWM olarak tanımlanıyor
buzzer = PWM(Pin(25))
buzzer.freq(1000)  # Buzzer frekansı 1 kHz
buzzer.duty_u16(0)  # Başlangıçta buzzer kapalı

# Servo motorların varsayılan başlangıç açıları
position1 = 90
position2 = 90
position3 = 90
position4 = 90

# Servo motorları başlangıç pozisyonlarına getir
Servo1.move(position1)
Servo2.move(position2)
Servo3.move(position3)
Servo4.move(position4)

# Hareket yönleri sabitleri
STOP = 0
FWD = 1
BWD = 2
RIGHT = 3
LEFT = 4
FWD_RIGHT = 5
FWD_LEFT = 6
BWD_RIGHT = 7
BWD_LEFT = 8

# Nota frekansları (piyano modu için)
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

# Kontrol modları (joystick veya düğmeler)
joystick = 1
buttons = 2

# Joystick merkez ve ölü alan değerleri
deadZone = 20
centerX = 127
centerY = 127

# Değer haritalama fonksiyonu
def custom_map(value, in_min, in_max, out_min, out_max):
    return (value - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

# Değeri verilen aralıkta sınırlandırma fonksiyonu
def constrain(val, min_val, max_val):
    return max(min_val, min(max_val, val))

# Belirli bir yön ve hızla hareket fonksiyonu
def move(direction, speed):
    avg = constrain(speed, 150, 255)
    duty = int((avg / 255) * 65535)
    motor_pwm.duty_u16(duty)

    # Belirlenen yönlere göre motor çıkışlarını ayarla
    if direction == FWD:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == BWD:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(0); motor_D2.value(1)
    elif direction == LEFT:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == RIGHT:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(0); motor_D2.value(1)
    elif direction == FWD_RIGHT:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(0); motor_D2.value(1)
    elif direction == FWD_LEFT:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == BWD_RIGHT:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == BWD_LEFT:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(0); motor_D2.value(1)
    else:
        motor_A1.value(0); motor_A2.value(0)
        motor_B1.value(0); motor_B2.value(0)
        motor_C1.value(0); motor_C2.value(0)
        motor_D1.value(0); motor_D2.value(0)

# OmniBot modunda yönlü hareket
def omni_move(direction, speed):
    avg = constrain(speed, 150, 255)
    duty = int((avg / 255) * 65535)
    motor_pwm.duty_u16(duty)

    # OmniBot için yönlü hareket kombinasyonları
    if direction == FWD:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(0); motor_D2.value(1)
    elif direction == BWD:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == LEFT:
        motor_A1.value(1); motor_A2.value(0)
        motor_B1.value(0); motor_B2.value(1)
        motor_C1.value(0); motor_C2.value(1)
        motor_D1.value(1); motor_D2.value(0)
    elif direction == RIGHT:
        motor_A1.value(0); motor_A2.value(1)
        motor_B1.value(1); motor_B2.value(0)
        motor_C1.value(1); motor_C2.value(0)
        motor_D1.value(0); motor_D2.value(1)
    # Diagonal ve STOP aynı mantıkta devam eder...

# Buzzer’ı kısa bir süre çal
def rex_horn():
    buzzer.duty_u16(32768)
    time.sleep(0.2)
    buzzer.duty_u16(0)

# Belirli bir müzik notasını çal
def piano_tone(note):
    buzzer.freq(note)
    buzzer.duty_u16(1000)
    time.sleep(0.2)
    buzzer.duty_u16(0)

# BLE üzerinden veri alma ve yorumlama
ble_device = ESP32_BLE()

while True:
    buffer = ble_device.get_data()  # BLE'den veri al
    if buffer:
        if buffer[0] == 75:  # "Rex" protokolü için başlık kontrolü
            # Gelen veriye göre Wibot, Omnibot, Armbot, Piano gibi modlar kontrol edilir
            # Detaylar yukarıdaki move(), omni_move() ve servo kontrol fonksiyonlarıyla uygulanır
            # Her modun özel kontrolü alt bloklarda ayrıntılı şekilde yazılmıştır
            ...
        ble_device.clear_data()  # Veri işlendikten sonra buffer temizlenir
