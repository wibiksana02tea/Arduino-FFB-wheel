# Firmware Roda Umpan Balik Gaya untuk STM32 (Versi Arduino)

Proyek ini adalah porting dari firmware `brWheel_my` untuk berjalan pada mikrokontroler STM32 menggunakan kerangka kerja Arduino.

## Persyaratan

1.  **Arduino IDE**: Versi 1.8.x atau yang lebih baru.
2.  **Inti Arduino STM32**: Dukungan papan untuk STM32.
3.  **Papan STM32**: Papan pengembangan STM32 seperti "Blue Pill" (STM32F103C8) atau papan Nucleo.
4.  **Programmer ST-Link**: Diperlukan untuk mengunggah firmware ke sebagian besar papan STM32.

## Langkah 1: Instal Dukungan Papan STM32 di Arduino IDE

1.  Buka Arduino IDE.
2.  Buka `File > Preferences`.
3.  Di bidang "Additional Boards Manager URLs", tambahkan URL berikut:
    ```
    http://dan.drown.org/stm32duino/package_STM32duino_index.json
    ```
4.  Klik "OK".
5.  Buka `Tools > Board > Boards Manager...`.
6.  Cari "STM32F1xx/GD32F1xx" dan instal.

## Langkah 2: Konfigurasi dan Kompilasi

1.  **Buka Proyek**: Buka berkas `brWheel_Arduino_STM32.ino` di Arduino IDE.
2.  **Pilih Papan**: Buka `Tools > Board` dan pilih papan STM32 Anda yang benar di bawah "STM32 Boards".
    *   Untuk "Blue Pill", pilih `Generic STM32F103C series`.
3.  **Konfigurasi Pengaturan Papan**:
    *   **Variant**: `STM32F103C8` (atau yang sesuai dengan chip Anda).
    *   **Upload method**: `STLink`.
    *   **USB support**: `CDC (generic serial)` (jika Anda memerlukan output `Serial.print`).
4.  **Edit `Config.h`**:
    *   Tinjau berkas `Config.h` di dalam proyek.
    *   Sesuaikan penetapan `_PIN` agar sesuai dengan cara Anda menyambungkan sensor, tombol, dan driver motor ke papan STM32 Anda.
5.  **Instal Pustaka**:
    *   Buka `Sketch > Include Library > Manage Libraries...`.
    *   Instal pustaka-pustaka berikut jika belum ada:
        *   `Adafruit ADS1X15`
        *   `Adafruit MCP4725`
        *   Pustaka `HX711_ADC` (mungkin perlu diinstal secara manual).
6.  **Kompilasi**:
    *   Klik tombol "Verify" (tanda centang) untuk mengompilasi proyek. Ini akan memastikan semua dependensi sudah benar sebelum mengunggah.

## Langkah 3: Unggah Firmware

1.  **Hubungkan ST-Link**: Hubungkan programmer ST-Link ke pin SWD (SWDIO, SWCLK, GND, 3.3V) pada papan STM32 Anda.
2.  **Hubungkan ke Komputer**: Hubungkan ST-Link ke komputer Anda melalui USB.
3.  **Unggah**: Klik tombol "Upload" di Arduino IDE. IDE akan menggunakan ST-Link untuk mem-flash firmware ke mikrokontroler STM32.

Setelah pengunggahan selesai, papan STM32 akan di-boot ulang dan harus dijumlahkan sebagai joystick umpan balik gaya di komputer Anda.
