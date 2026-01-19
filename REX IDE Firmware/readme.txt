+-----------------------------------------+
|            Robotistan INC - 2024        |
+-----------------------------------------+
Robotistan INC programlama aracına hoş geldiniz! Bu araç, MicroPython'u ESP32 veya REX Board'unuza kolayca yüklemenizi sağlar.

1- ESP32'nizi bağlayın: ESP32 veya REX Board'unuzu USB aracılığıyla bilgisayarınıza takın.
2- .bat dosyasını çalıştırın: uploadFirmware.bat dosyasına çift tıklayarak çalıştırın.
3- COM portunuzu seçin: İstendiğinde, COM portunuzu yazın (örneğin: COM3).
4- Araç, flash'ı otomatik olarak silecek ve MicroPython'u cihazınıza yazacaktır.
	- Eğer "Failed to get PID of a device on COM*, using standard reset sequence." mesajını görürseniz 
	ve noktalar sürekli olarak ekranda beliriyorsa ("...."), cihazınızdaki "BOOT" tuşuna basılı tutarak 
	yazılımı tekrar yüklemeyi deneyin.
5- İşlem tamamlandığında, işlem tamamlanmış olur!

Hepsi bu kadar! Artık ESP32'nizde MicroPython kullanmaya hazırsınız!

Önemli Notlar:
Windows Uyarısı: Eğer bat dosyasını çalıştırırken bir Windows uyarısı görürseniz, Daha Fazla Bilgi kısmına tıklayın ve ardından Yine de Çalıştır seçeneğine tıklayarak işlemi devam ettirin.
COM Portu Görünmüyorsa: Cihazınızı COM portları listesinde göremiyorsanız, CH340 sürücüsünü yüklemeniz gerekebilir. CH341SER.EXE dosyasını çalıştırarak Install tuşuna basarak sürücüyü yükleyebilirsiniz.
