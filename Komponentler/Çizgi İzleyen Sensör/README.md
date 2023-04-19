## Çizgi İzleyen Sensör
Çizgi izleyen sensörler, kızılötesi (IR) ışık yayarak ve ışık seviyelerini algılayarak siyah bir çizginin varlığını algılar. Bunu iki bileşen kullanarak yaparlar:

- Bir emitör
- Bir ışık sensörü (alıcı)

<p align="center">

  
  <img src="https://user-images.githubusercontent.com/112697142/230352474-a23c896c-584f-4dc7-a559-e2915b4881b6.gif" alt="animated" />
  
  
</p>


### Çizgi sensörü nasıl kullanılır?
Çizgi izleyen sensörün nasıl çalıştığını biraz daha biliyorsunuz, şimdi onu nasıl kullanacağınızı öğrenmelisiniz. Çizgi izleyici sensöründe ayrıca, kullanmakta olduğunuz Raspberry Pi'ye bağlamanız gereken bir dizi pin bulunur:

- Cihaza güç sağlamak için VCC'nin 3,3 ile 5V arasında bir gerilime bağlanması gerekir.
- GND, devreyi tamamlamak için gerekli olan topraklama pinidir.
- AO analog çıkıştır (bu pin Raspberry Pi ile çalışmaz.).
- DO dijital çıkış pinidir (bu pin Raspberry Pi ile çalışacaktır.).
