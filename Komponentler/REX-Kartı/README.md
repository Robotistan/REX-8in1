# REX Kartı

<p align="center">
  <img src="https://user-images.githubusercontent.com/112697142/230360152-12d52dec-c5a4-4487-ae4a-e29177675dd2.gif" alt="animated" />
</p>




# ESP32 WROOM 32E Pin Diyagramı



<p align="center">
  <img src="https://user-images.githubusercontent.com/78815495/232464806-6abdce40-22c8-40ba-807a-c8adba2092a3.png" alt="animated" />
</p>

#### Uyarı! Kartı kullanırken 18650 pil kullanıyorsanız lütfen harici pil bağlamayın.


### Soketlere Bağlı Olan Pinler

| Motor-A   | GPIO     | Not                           |
| :-------- | :------- | :-------------------------    |
| `M_A1`    | `15`     | **Gerekli**. API anahtarınız. |
| `M_A2`    | `23`     | **Gerekli**. API anahtarınız. |

| Motor-B   | GPIO     | Not                           |
| :-------- | :------- | :-------------------------    |
| `M_B1`    | `32`     | **Gerekli**. API anahtarınız. |
| `M_B2`    | `33`     | **Gerekli**. API anahtarınız. |

| Motor-C   | GPIO     | Not                           |
| :-------- | :------- | :-------------------------    |
| `M_c1`    | `4`      | **Gerekli**. API anahtarınız. |
| `M_c2`    | `5`      | **Gerekli**. API anahtarınız. |

| Motor-D   | GPIO     | Not                           |
| :-------- | :------- | :-------------------------    |
| `M_D1`    | `14`     | **Gerekli**. API anahtarınız. |
| `M_D2`    | `27`     | **Gerekli**. API anahtarınız. |


| Servo Pins   | GPIO     | Not                           |
| :--------    | :------- | :-------------------------    |
| `Servo-1`    | `2`      | **Gerekli**. API anahtarınız. |
| `Servo-2`    | `26`     | **Gerekli**. API anahtarınız. |
| `Servo-3`    | `18`     | **Gerekli**. API anahtarınız. |
| `Servo-4`    | `19`     | **Gerekli**. API anahtarınız. |

| IR Sensor | GPIO     |       Not                     |
| :-------- | :------- | :-------------------------    |
| `TX`      | `1`      | **Gerekli**. API anahtarınız. |
| `RX`      | `3`      | **Gerekli**. API anahtarınız. |

| Ulstrasonic Sensor   | GPIO      |       Not                     |
| :--------            | :-------  | :-------------------------    |
| `TRIG`               | `17`      | **Gerekli**. API anahtarınız. |
| `ECHO`               | `16`      | **Gerekli**. API anahtarınız. |

| I2C        | GPIO      |       Not                     |
| :--------  | :-------  | :-------------------------    |
| `SCL`      | `22`      | **Bağlı** Sarı Kablo          |
| `SDA`      | `21`      | **Bağlı** ''Beyaz Kablo''     |

| Buzzer     | GPIO      |       Not                     |
| :--------  | :-------  | :-------------------------    |
| `Buzzer`   | `25`      | **Gerekli**. API anahtarınız. |

| MPU6050    | GPIO      |       Not                    |
| :--------  | :-------  | :-------------------------   |
| `SCL`      | `22`      | **Bağlı** Sarı Kablo         |
| `SDA`      | `21`      | **Bağlı** Beyaz Kablo        |
| `INT`      | `13`      | **Bağlı** Sarı Kablo         |
