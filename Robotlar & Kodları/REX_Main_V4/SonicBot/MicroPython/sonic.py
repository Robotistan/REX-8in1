from machine import Pin, ADC, PWM
import time
from rex import HCSR04  # HCSR04 ultrasonik sensör kütüphanesini içeri aktar

# motorA pinleri tanımlanıyor
motor_A1 = PWM(Pin(15))
motor_A1.duty_u16(0)
motor_A2 = PWM(Pin(23))
motor_A2.duty_u16(0)

# motorB pinleri tanımlanıyor
motor_B1 = PWM(Pin(32))
motor_B1.duty_u16(0)
motor_B2 = PWM(Pin(33))
motor_B2.duty_u16(0)

# motorC pinleri tanımlanıyor
motor_C1 = PWM(Pin(5))
motor_C1.duty_u16(0)
motor_C2 = PWM(Pin(4))
motor_C2.duty_u16(0)

# motorD pinleri tanımlanıyor
motor_D1 = PWM(Pin(14))
motor_D1.duty_u16(0)
motor_D2 = PWM(Pin(27))
motor_D2.duty_u16(0)

# buzzer pini tanımlanıyor
buzzer = Pin(25, Pin.OUT)

# ultrasonik sensör nesnesi oluşturuluyor (trigger: GP17, echo: GP16)
sensor = HCSR04(trigger_pin=17, echo_pin=16, echo_timeout_us=10000)

# varsayılan motor hızı tanımlanıyor (0 ile 65535 arasında)
MotorSpeed = 50000

# ileri hareket fonksiyonu
def forward(speed):
   motor_A1.duty_u16(speed)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(speed)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# durdurma fonksiyonu
def stop():
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(0)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(0)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(0)

# geri hareket fonksiyonu
def backward(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(0)
   motor_C2.duty_u16(speed)
   motor_D1.duty_u16(0)
   motor_D2.duty_u16(speed)

# sola dönüş fonksiyonu
def left(speed):
   motor_A1.duty_u16(0)
   motor_A2.duty_u16(speed)
   motor_B1.duty_u16(0)
   motor_B2.duty_u16(speed)
   motor_C1.duty_u16(speed)
   motor_C2.duty_u16(0)
   motor_D1.duty_u16(speed)
   motor_D2.duty_u16(0)

# ana döngü
while True:
    distance = sensor.distance_cm()  # Ultrasonik sensör ile mesafe ölçülüyor
    print(distance)  # Ölçülen mesafe seri ekrana yazdırılıyor
    
    if distance > 12:
        # Eğer önünde 12 cm'den fazla mesafe varsa ileri git
        forward(MotorSpeed)
    else:
        # Eğer 12 cm'den az mesafe varsa:
        backward(MotorSpeed)  # Geri git
        buzzer.value(1)       # Buzzer'ı aktif et
        time.sleep(0.3)       # 0.3 saniye bekle
        buzzer.value(0)       # Buzzer'ı kapat
        left(MotorSpeed)      # Sola dön
        time.sleep(0.2)       # 0.2 saniye dön
        stop()                # Durdur
