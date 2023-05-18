# Sistem Monitoring Polusi Menggunakan Platform Thingspeak & MQTT
Menampilkan tingkat intensitas suara dalam desibel disertai dengan kategori kebisingan, terdapat 3 tingkat kebisingan yang diatur dalam program: sunyi (<60db), sedang (>60db sampai dengan <85db), dan tinggi (>85db). Masing-masing tingkat kebisingan yang diterima sensor ditandai dengan 3 buah LED yang berwarna merah, kuning, dan hijau. Setiap input yang diterima oleh sensor terhubung dengan platform dan panel MQTT.Projek ini merupakan post test 3 praktikum mata kuliah Internet of Things di Univeritas Mulawarman.

## Metode IoT
Monitoring dilakukan melalui web server yang sudah didesain menggunakan bahasa pemrograman HTML, CSS, dan JS. Data yang ditampilkan di web server berupa grafik yang diambil dari platform ThingSpeak yang terhubung dengan device IoT menggunakan API. Monitoring juga dilakukan melalui panel MQTT pada handphone dengan cara mensubscribe program pada aplikasi IoT Panel MQTT dengan topik yang sudah ditentukan.

## Komponen yang digunakan
1. ESP8266
2. Breadboard
3. Resistor
4. LED (warna merah, kuning, dan hijau)
5. LCD I2C 16x2
6. Microphone Adafruit MAX9814
7. Push Button
8. Kabel Jumper
9. Kabel micro-USB

## Board Schematic
![Schematic Board PostTest 3_bb](https://github.com/AlyaniNS/posttest3-praktikum-iot-unmul/assets/74224380/1e39fbee-ce16-4e30-af76-249456ddd24e)


## Pembagian Tugas
No|Nim       |Nama                            |Tugas                        |
--|----------|--------------------------------|-----------------------------|
1 |1915026021|Muhammad Dicky Rizqia Nur Achmar|Konfigurasi Thingspeak & MQTT|
2 |2009106100|Alyani Noor Septalia            |Konfigurasi Program          |
3 |2009106112|Adrian Tasmin                   |Membuat Rangkaian            |
