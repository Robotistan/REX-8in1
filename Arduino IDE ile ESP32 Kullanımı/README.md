## Arduino IDE ile ESP32 Kullanımı
ESP32, yüksek işlem gücü ve kullanışlı fonksiyonları nedeniyle birçok mikrodenetleyici ve geliştirme kartı arasında oldukça fazla tercih edilmektedir. ESP32'yi nasıl kullanacağımızı öğrenelim ve ESP32 kütüphanesini kuralım.

## ESP32'yi Arduino IDE'ye Kurulumu
ESP32 kartını Arduino IDE'nize kurmak için aşağıdaki adımları izleyin.

- Arduino IDE'de Dosya> Tercihler'e gidin

![Untitled](https://user-images.githubusercontent.com/112697142/208438239-c10e7e7a-a204-4795-9554-55dcad767a35.png)

- "Ek Pano Yöneticisi URL'leri" alanına aşağıdakini yazın

 | https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json |
 | ------------------------------------------------------------------------------------------- |

 
![Untitledd](https://user-images.githubusercontent.com/112697142/208445667-963ec70d-b191-49ed-a9c4-1906bcc809f1.png)

- Pano Yöneticisi'ne tıklayın

![arduinoboard](https://user-images.githubusercontent.com/112697142/209644042-b65e3e12-ae33-4756-a7eb-a47da9d6af19.png)

- ESP32'yi aradıktan sonra kur düğmesine tıklayarak ESP32'yi kurun. Birkaç saniye içinde hazır olacak.

![ardubo](https://user-images.githubusercontent.com/112697142/209644481-aad29edf-534c-41ae-ba10-a866addfa134.png)

## Kurulumu Test Edelim
Arduino IDE'yi açtıktan sonra ESP32 kartınızı bilgisayarınıza takın ve aşağıdaki adımları takip edin.

- ##### 1. Adım

![boarddoit](https://user-images.githubusercontent.com/112697142/209684706-a95c8eb0-d572-4b0c-b486-d9ba45b4d016.png)

- ##### 2. Adım

![ardcom](https://user-images.githubusercontent.com/112697142/209685031-93f13f3e-6c41-4cdd-b51b-ad0d41279489.png)

- ##### 3. Adım

![ardexam](https://user-images.githubusercontent.com/112697142/209685993-18cf4e36-fae3-4b21-a506-23f1f86ed91e.png)

- ##### Arduino IDE'nizde yeni bir sayfa açılır.

![S](https://user-images.githubusercontent.com/112697142/217796920-13df56dd-abad-439b-81a8-66ad97f4d7b0.png)

- ##### Yükleme butonuna tıklayın
- ##### Her şey beklendiği gibi giderse, “Yükleme tamamlandı” mesajını göreceğiz.

![Screenshotfsdf 2023-02-09 141242](https://user-images.githubusercontent.com/112697142/217797281-2c65781a-b6c8-404d-bdd6-965a6a3c6ab4.png)

- ##### Arduino IDE Serial Monitor'ü 115200 baud hızında açın.

![image](https://user-images.githubusercontent.com/112697142/217797641-1f3f525a-6821-4857-b283-694af1301d79.png)

