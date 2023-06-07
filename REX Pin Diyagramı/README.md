# REX 8in1 Anakart

<p align="center">
  <img src="https://user-images.githubusercontent.com/112697142/230360152-12d52dec-c5a4-4487-ae4a-e29177675dd2.gif" alt="animated" />
</p>

<p align="center">
360° Preview
</p>

# ESP32 WROOM 32E Pin Diyagramı


#### Uyarı! Anakartı kullanırken 18650 pil kullanıyorsanız lütfen harici pil bağlamayın!



<p align="center">
  <img src="https://user-images.githubusercontent.com/78815495/232464806-6abdce40-22c8-40ba-807a-c8adba2092a3.png" alt="animated" />
</p>

## Pin Bağlantı Tabloları
### DC Motor Pinleri
| Motor-A   | GPIO     | Açıklama                      | - | Motor-B   | GPIO     | Açıklama                      |
| :-------- | :------- | :-------------------------    |:--| :-------- | :------- | :-------------------------    |
| `M_A1`    | `15`     | Forward.                      | - | `M_B1`    | `32`     | Forward.                      |
| `M_A2`    | `23`     | Backward.                     | - | `M_B2`    | `33`     | Backward.                     |

| Motor-C   | GPIO     | Açıklama                      | - | Motor-D   | GPIO     | Açıklama                      |
| :-------- | :------- | :-------------------------    |:- | :-------- | :------- | :-------------------------    |
| `M_C1`    | `4`      | Forward.                      | - | `M_D1`    | `14`     | Forward.                      |
| `M_C2`    | `5`      | Backward.                     | - | `M_D2`    | `27`     | Backward.                     |

### Servo Motor Pinleri
| Servo Pins   | GPIO     | Açıklama                   | -  | Servo Pins   | GPIO     | Açıklama                 |
| :--------    | :------- | :------------------------- |:-| :--------    | :------- | :------------------------- |
| `Servo-1`    | `2`      | Powered by 6V | - | `Servo-3`    | `18`     | Powered by 6V |
| `Servo-2`    | `26`     | Powered by 6V | - | `Servo-4`    | `19`     | Powered by 6V |

### IR Sensor Pinleri
| IR Sensor | GPIO     |       Açıklama                |
| :-------- | :------- | :-------------------------    |
| `A0`      | `34`      | Connected to left sensor when loked from top. |
| `A1`      | `35`      | Connected to right sensor when loked from top. |
### Ultrasonik Sensor Pinleri
| Ultrasonik Sensor    | GPIO      |                                    Açıklama                                    |
| :--------            | :-------  | :-------------------------                                                     |
| `TRIG`               | `17`      | Pin is used to trigger ultrasonic sound pulses                                 |
| `ECHO`               | `16`      | By measuring the time the Echo pin stays high, the distance can be calculated. |
### I2C Pinleri
| I2C        | GPIO      |       Açıklama                |
| :--------  | :-------  | :-------------------------    |
| `SCL`      | `22`      | **Connected** Yellow Connection Cable. |
| `SDA`      | `21`      | **Connected** White Connection Cable. |
### Buzzer Pinleri
| Buzzer     | GPIO      |       Açıklama                |
| :--------  | :-------  | :-------------------------    |
| `Buzzer`   | `25`      |  **Connected** pin 25. |
### MPU6050 Pinleri
| MPU6050    | GPIO      |       Açıklama               |
| :--------  | :-------  | :-------------------------   |
| `SCL`      | `22`      | I2C Communication |
| `SDA`      | `21`      | I2C Communication |
| `INT`      | `13`      | interrupt pin     |


</p>
<p align="center">
  <img src="https://user-images.githubusercontent.com/78815495/232464533-1807c9cc-87b0-4529-b5ec-bb78b5b8704c.jpg" alt="animated" />
</p>

