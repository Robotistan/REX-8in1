// REX 8in1 Omni Bot için ESP32 kodu
// ESP32 Board Version: 2.0.11 kullanılmalıdır

#include <DabbleESP32.h>
#include <Arduino.h>

// Motor pinlerinin tanımlanması
#define MotorPWM 13	//Pwm

#define MotorA1 15 // Forward
#define MotorA2 23 // Backward

#define MotorB1 32 // Forward
#define MotorB2 33 // Backward

#define MotorC1 5 // Forward
#define MotorC2 4 // Backward

#define MotorD1 27 // Forward
#define MotorD2 14 // Backward

// Buzzer pini (horn)
#define horn 25

// Motor hız seviyeleri (düşük, orta, yüksek)
#define low_s 100
#define mid_s 150
#define high_s 250

// PWM özellikleri
const int freq = 50;        // PWM frekansı
const int resolution = 8;  // PWM çözünürlüğü (bit cinsinden)

// Her motor yönü için ayrı PWM kanalları atanır
const int PWMchannel_1 = 4;
const int PWMchannel_2 = 5;
const int PWMchannel_3 = 6;
const int PWMchannel_4 = 7;
const int PWMchannel_5 = 8;
const int PWMchannel_6 = 9;
const int PWMchannel_7 = 10;
const int PWMchannel_8 = 11;

void setup() {
  pinMode(horn, OUTPUT);

  pinMode(MotorPWM, OUTPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);

  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);
  
  // PWM yapılandırması ve pinlere atanması
  ledcSetup(PWMchannel_1, freq, resolution);
  ledcAttachPin(MotorA1, PWMchannel_1);

  ledcSetup(PWMchannel_2, freq, resolution);
  ledcAttachPin(MotorA2, PWMchannel_2);

  ledcSetup(PWMchannel_3, freq, resolution);
  ledcAttachPin(MotorB1, PWMchannel_3);

  ledcSetup(PWMchannel_4, freq, resolution);
  ledcAttachPin(MotorB2, PWMchannel_4);

  ledcSetup(PWMchannel_5, freq, resolution);
  ledcAttachPin(MotorC1, PWMchannel_5);

  ledcSetup(PWMchannel_6, freq, resolution);
  ledcAttachPin(MotorC2, PWMchannel_6);

  ledcSetup(PWMchannel_7, freq, resolution);
  ledcAttachPin(MotorD1, PWMchannel_7);

  ledcSetup(PWMchannel_8, freq, resolution);
  ledcAttachPin(MotorD2, PWMchannel_8);

  Serial.begin(115200); 
  Dabble.begin("REX_ROBOT"); // Bluetooth adı belirlenir
}

void loop() {
  delay(20);
  Dabble.processInput();  // Bluetooth verileri alınır

  int a = GamePad.getAngle();    // Joystick açısı
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');

  int b = GamePad.getRadius();    // Joystick yarıçapı
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');

  if (GamePad.isCrossPressed())
  {
    rex_horn();
  }

  if (GamePad.isSquarePressed())
  {
    Serial.print("Square");
  }
  digitalWrite(MotorPWM, HIGH);

  //Forward
  if (a > 60 && a < 120 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, low_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, low_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);     //MotorB2

    ledcWrite(PWMchannel_5, low_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, low_s); //MotorD1
    ledcWrite(PWMchannel_8, 0);     //MotorD2
  }

  else if (a > 60 && a < 120 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, mid_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, mid_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);     //MotorB2

    ledcWrite(PWMchannel_5, mid_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, mid_s); //MotorD1
    ledcWrite(PWMchannel_8, 0);     //MotorD2
  }

  else if (a > 60 && a < 120 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, high_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, high_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);     //MotorB2

    ledcWrite(PWMchannel_5, high_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, high_s); //MotorD1
    ledcWrite(PWMchannel_8, 0);     //MotorD2
  }

  //Backward
  else if (a > 240 && a < 300 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, 0);     //MotorA1
    ledcWrite(PWMchannel_2, low_s); //MotorA2

    ledcWrite(PWMchannel_3, 0);     //MotorB1
    ledcWrite(PWMchannel_4, low_s); //MotorB2

    ledcWrite(PWMchannel_5, 0);     //MotorC1
    ledcWrite(PWMchannel_6, low_s); //MotorC2

    ledcWrite(PWMchannel_7, 0);     //MotorD1
    ledcWrite(PWMchannel_8, low_s); //MotorD2
  }

  else if (a > 240 && a < 300 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, 0);     //MotorA1
    ledcWrite(PWMchannel_2, mid_s); //MotorA2

    ledcWrite(PWMchannel_3, 0);     //MotorB1
    ledcWrite(PWMchannel_4, mid_s); //MotorB2

    ledcWrite(PWMchannel_5, 0);     //MotorC1
    ledcWrite(PWMchannel_6, mid_s); //MotorC2

    ledcWrite(PWMchannel_7, 0);     //MotorD1
    ledcWrite(PWMchannel_8, mid_s); //MotorD2
  }

  else if (a > 240 && a < 300 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, 0);      //MotorA1
    ledcWrite(PWMchannel_2, high_s); //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, high_s); //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, high_s); //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, high_s); //MotorD2
  }

  //Right (0-30)
  else if (a >= 0 && a < 30 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, low_s); //MotorA1  
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, 0); //MotorB1
    ledcWrite(PWMchannel_4, low_s); //MotorB2

    ledcWrite(PWMchannel_5, low_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, low_s);     //MotorD2
  }

  else if (a >= 0 && a < 30 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, mid_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, 0); //MotorB1
    ledcWrite(PWMchannel_4, mid_s);     //MotorB2

    ledcWrite(PWMchannel_5, mid_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, mid_s);     //MotorD2
  }

  else if (a >= 0 && a < 30 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, high_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, 0); //MotorB1
    ledcWrite(PWMchannel_4, high_s);     //MotorB2

    ledcWrite(PWMchannel_5, high_s); //MotorC1
    ledcWrite(PWMchannel_6, 0);     //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, high_s);     //MotorD2
  }
  //Right (330-360)
  else if (a > 330 && a < 360 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, low_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, 0);     //MotorB1
    ledcWrite(PWMchannel_4, low_s); //MotorB2

    ledcWrite(PWMchannel_5, low_s);     //MotorC1
    ledcWrite(PWMchannel_6, 0); //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, low_s);     //MotorD2
  }

  else if (a > 330 && a < 360 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, mid_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);     //MotorA2

    ledcWrite(PWMchannel_3, 0);     //MotorB1
    ledcWrite(PWMchannel_4, mid_s); //MotorB2

    ledcWrite(PWMchannel_5, mid_s);     //MotorC1
    ledcWrite(PWMchannel_6, 0); //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, mid_s);     //MotorD2
  }

  else if (a > 330 && a < 360 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, high_s); //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, high_s); //MotorB2

    ledcWrite(PWMchannel_5, high_s);      //MotorC1
    ledcWrite(PWMchannel_6, 0); //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, high_s);      //MotorD2
  }

  //Left
  else if (a > 150 && a < 210 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, 0);     //MotorA1
    ledcWrite(PWMchannel_2, low_s); //MotorA2

    ledcWrite(PWMchannel_3, low_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);     //MotorB2

    ledcWrite(PWMchannel_5, 0); //MotorC1
    ledcWrite(PWMchannel_6, low_s);     //MotorC2

    ledcWrite(PWMchannel_7, low_s);     //MotorD1
    ledcWrite(PWMchannel_8, 0); //MotorD2
  }

  else if (a > 150 && a < 210 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, 0);     //MotorA1
    ledcWrite(PWMchannel_2, mid_s); //MotorA2

    ledcWrite(PWMchannel_3, mid_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);     //MotorB2

    ledcWrite(PWMchannel_5, 0); //MotorC1
    ledcWrite(PWMchannel_6, mid_s);     //MotorC2

    ledcWrite(PWMchannel_7, mid_s);     //MotorD1
    ledcWrite(PWMchannel_8, 0); //MotorD2
  }

  else if (a > 150 && a < 210 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, 0);       //MotorA1
    ledcWrite(PWMchannel_2, high_s);  //MotorA2

    ledcWrite(PWMchannel_3, high_s); //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, 0); //MotorC1
    ledcWrite(PWMchannel_6, high_s);      //MotorC2

    ledcWrite(PWMchannel_7, 0); //MotorD1
    ledcWrite(PWMchannel_8, high_s);      //MotorD2
  }

  //Right-Forward
  else if (a >= 30 && a <= 60 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, low_s);  //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, low_s);  //MotorD1
    ledcWrite(PWMchannel_8, 0);      //MotorD2
  }

  else if (a >= 30 && a <= 60 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, mid_s);  //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, mid_s);  //MotorD1
    ledcWrite(PWMchannel_8, 0);      //MotorD2
  }

  else if (a >= 30 && a <= 60 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, high_s);   //MotorA1
    ledcWrite(PWMchannel_2, 0);        //MotorA2

    ledcWrite(PWMchannel_3, 0);        //MotorB1
    ledcWrite(PWMchannel_4, 0);        //MotorB2

    ledcWrite(PWMchannel_5, 0);        //MotorC1
    ledcWrite(PWMchannel_6, 0);        //MotorC2

    ledcWrite(PWMchannel_7, high_s);   //MotorD1
    ledcWrite(PWMchannel_8, 0);        //MotorD2
  }

  //Left-Forward
  else if (a >= 120 && a <= 150 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, 0);      //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, low_s);  //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, low_s);  //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, 0);      //MotorD2
  }

  else if (a >= 120 && a <= 150 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, 0);       //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, mid_s);  //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, mid_s);  //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, 0);      //MotorD2
  }

  else if (a >= 120 && a <= 150 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, 0);         //MotorA1
    ledcWrite(PWMchannel_2, 0);        //MotorA2

    ledcWrite(PWMchannel_3, high_s);   //MotorB1
    ledcWrite(PWMchannel_4, 0);        //MotorB2

    ledcWrite(PWMchannel_5, high_s);   //MotorC1
    ledcWrite(PWMchannel_6, 0);        //MotorC2

    ledcWrite(PWMchannel_7, 0);        //MotorD1
    ledcWrite(PWMchannel_8, 0);        //MotorD2
  }

  //Right-Backward
  else if (a >= 210 && a <= 240 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, 0);      //MotorA1
    ledcWrite(PWMchannel_2, low_s);  //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, low_s);  //MotorD2
  }

  else if (a >= 210 && a <= 240 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, 0);      //MotorA1
    ledcWrite(PWMchannel_2, mid_s);  //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, 0);      //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, mid_s);  //MotorD2
  }

  else if (a >= 210 && a <= 240 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, 0);         //MotorA1
    ledcWrite(PWMchannel_2, high_s);    //MotorA2

    ledcWrite(PWMchannel_3, 0);         //MotorB1
    ledcWrite(PWMchannel_4, 0);         //MotorB2

    ledcWrite(PWMchannel_5, 0);         //MotorC1
    ledcWrite(PWMchannel_6, 0);         //MotorC2

    ledcWrite(PWMchannel_7, 0);         //MotorD1
    ledcWrite(PWMchannel_8, high_s);    //MotorD2
  }

  //Right-Backward
  else if (a >= 300 && a <= 330 && b >= 1 && b <= 3)
  {
    ledcWrite(PWMchannel_1, 0);       //MotorA1
    ledcWrite(PWMchannel_2, 0);       //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, low_s);  //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, 0);      //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, low_s);  //MotorD2
  }

  else if (a >= 300 && a <= 330 && b > 3 && b <= 5)
  {
    ledcWrite(PWMchannel_1, 0);      //MotorA1
    ledcWrite(PWMchannel_2, 0);      //MotorA2

    ledcWrite(PWMchannel_3, 0);      //MotorB1
    ledcWrite(PWMchannel_4, mid_s);  //MotorB2

    ledcWrite(PWMchannel_5, 0);      //MotorC1
    ledcWrite(PWMchannel_6, mid_s);  //MotorC2

    ledcWrite(PWMchannel_7, 0);      //MotorD1
    ledcWrite(PWMchannel_8, 0);      //MotorD2
  }

  else if (a >= 300 && a <= 330 && b > 5 && b <= 7)
  {
    ledcWrite(PWMchannel_1, 0);         //MotorA1
    ledcWrite(PWMchannel_2, 0);         //MotorA2

    ledcWrite(PWMchannel_3, 0);         //MotorB1
    ledcWrite(PWMchannel_4, high_s);    //MotorB2

    ledcWrite(PWMchannel_5, 0);         //MotorC1
    ledcWrite(PWMchannel_6, high_s);    //MotorC2

    ledcWrite(PWMchannel_7, 0);         //MotorD1
    ledcWrite(PWMchannel_8, 0);         //MotorD2
  }
  /////////////////////////////DUR////////////////////////////////////
  else
  {
    digitalWrite(MotorPWM, LOW);

    ledcWrite(PWMchannel_1, 0);         //MotorA1
    ledcWrite(PWMchannel_2, 0);         //MotorA2

    ledcWrite(PWMchannel_3, 0);         //MotorB1
    ledcWrite(PWMchannel_4, 0);         //MotorB2

    ledcWrite(PWMchannel_5, 0);         //MotorC1
    ledcWrite(PWMchannel_6, 0);         //MotorC2

    ledcWrite(PWMchannel_7, 0);         //MotorD1
    ledcWrite(PWMchannel_8, 0);         //MotorD2
  }
}

void rex_horn() {
  for(int i=0; i<50; i++){
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
}
