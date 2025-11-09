# Firmware Roda Umpan Balik Gaya Klon Logitech G29 untuk STM32

Proyek ini adalah porting dari firmware `brWheel_my` yang dimodifikasi untuk meniru **Logitech G29 Driving Force Racing Wheel**. Ini memastikan kompatibilitas "plug-and-play" dengan sebagian besar game balap di PC.

## GUI Konfigurasi

Firmware ini kompatibel dengan GUI konfigurasi asli. Anda dapat menemukannya di sini:
*   **[Arduino FFB GUI](https://github.com/wibiksana02tea/Arduino-FFB-gui)**

## Persyaratan

1.  **Arduino IDE**: Versi 1.8.x atau yang lebih baru.
2.  **Inti Arduino STM32**: Dukungan papan untuk STM32.
3.  **Papan STM32**: Papan pengembangan STM32 (misalnya, "Blue Pill").
4.  **Programmer ST-Link**.

## Langkah 1: Modifikasi Definisi Papan Arduino

Agar klon ini berhasil, Anda **harus** mengedit berkas konfigurasi inti Arduino STM32 untuk menimpa VID (Vendor ID) dan PID (Product ID) default dengan milik Logitech.

1.  Temukan folder instalasi inti Arduino STM32 Anda. Ini biasanya berada di:
    *   Windows: `C:\Users\<YourUser>\AppData\Local\Arduino15\packages\STM32\hardware\stm32\<version>\`
    *   macOS/Linux: `~/.arduino15/packages/STM32/hardware/stm32/<version>/`
2.  Buka berkas `boards.txt` di editor teks.
3.  Temukan bagian untuk papan Anda (misalnya, cari `genericSTM32F103C.name=Generic STM32F103C series`).
4.  Di dalam bagian itu, tambahkan (atau edit) baris berikut untuk menyetel VID/PID Logitech G29:
    ```
    genericSTM32F103C.build.vid=0x046D
    genericSTM32F103C.build.pid=0xC260
    ```
5.  Simpan berkas `boards.txt` dan **restart Arduino IDE** agar perubahan diterapkan.

**PERINGATAN**: Perubahan ini akan membuat **semua proyek** yang Anda kompilasi untuk papan ini menggunakan VID/PID Logitech. Anda mungkin ingin membuat duplikat definisi papan jika Anda mengerjakan proyek USB lain.

## Langkah 2: Konfigurasi dan Kompilasi

1.  **Buka Proyek**: Buka berkas `brWheel_G29_Clone.ino`.
2.  **Pilih Papan**: Buka `Tools > Board` dan pilih papan STM32 Anda (`Generic STM32F103C series`).
3.  **Konfigurasi Pengaturan Papan**:
    *   **Variant**: `STM32F103C8`.
    *   **Upload method**: `STLink`.
    *   **Profil USB**: Pilih **`"HID (Joystick) + Serial"`**.
4.  **Edit `Config.h`**: Sesuaikan pin agar sesuai dengan kabel Anda.
5.  **Kompilasi**: Klik tombol "Verify".

## Langkah 3: Unggah Firmware

1.  Hubungkan ST-Link Anda dan klik "Upload".

Setelah diunggah, Windows/Linux sekarang akan mendeteksi perangkat Anda bukan sebagai joystick generik, tetapi sebagai **"Logitech G29 Driving Force Racing Wheel"**.
