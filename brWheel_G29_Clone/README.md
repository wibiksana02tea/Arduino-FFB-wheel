# Firmware Roda Umpan Balik Gaya Klon Logitech G29 untuk STM32

Proyek ini adalah porting dari firmware `brWheel_my` yang dimodifikasi untuk meniru **Logitech G29 Driving Force Racing Wheel**. Ini memastikan kompatibilitas "plug-and-play" dengan sebagian besar game balap di PC.

## GUI Konfigurasi

Firmware ini kompatibel dengan GUI konfigurasi asli. Anda dapat menemukannya di repositori berikut:

*   **[Arduino FFB GUI](https://github.com/wibiksana02tea/Arduino-FFB-gui)**

---

## **PENTING**: Wiring dan Perbedaan Pin

Firmware ini telah di-porting dari papan berbasis AVR (Arduino Leonardo/Pro Micro) ke platform STM32 yang lebih umum (contoh menggunakan "Blue Pill" STM32F103). Akibatnya, penetapan pin telah berubah secara signifikan.

**ANDA HARUS** memverifikasi bahwa wiring fisik Anda cocok dengan pin yang ditentukan dalam berkas `Config.h` dan `QuadEncoder.h`. Tabel di bawah ini menunjukkan perbedaan antara pinout asli dan pinout contoh baru untuk STM32.

| Fungsi | Pin Asli (Arduino Leonardo) | Pin Contoh Baru (STM32 "Blue Pill") | Catatan |
| :--- | :--- | :--- | :--- |
| **Pedal & Sumbu** | | | |
| Pedal Gas | `A0` | `PA0` | |
| Pedal Rem | `A1` | `PA1` | |
| Pedal Kopling | `A2` | `PA2` | |
| Rem Tangan | `A3` | `PA3` | |
| **Motor FFB** | | | |
| PWM Kiri (L) | `D9` | `PA8` | Memerlukan timer perangkat keras (TIM1) |
| PWM Kanan (R) | `D10` | `PA9` | Memerlukan timer perangkat keras (TIM1) |
| PWM Atas (U) | `D11` | `PA10` | Untuk sumbu FFB ke-2 |
| PWM Bawah (D) | `D5` | `PA11` | Untuk sumbu FFB ke-2 |
| Arah (DIR) | `D11` | `PB12` | Untuk mode PWM+DIR |
| **Tombol** | | | |
| Tombol 0-7 | Pin Digital & Analog Campuran | `PA4` - `PA7`, `PB0`, `PB1`, `PB10`, `PB11` | Lihat `Config.h` untuk detailnya |
| **Encoder** | | | |
| Encoder A | `D2` | `PB6` | **Konflik dengan I2C!** |
| Encoder B | `D3` | `PB7` | **Konflik dengan I2C!** |
| **Lain-lain** | | | |
| I2C SDA | `D2` | `PB7` | |
| I2C SCL | `D3` | `PB6` | |
| LED Status | `D13` | `PC13` | LED on-board |

**PERINGATAN KONFLIK PIN:** Pada banyak papan STM32 "Blue Pill", pin I2C default (`PB6`, `PB7`) adalah pin yang sama dengan yang digunakan untuk contoh encoder kuadratur. Jika Anda berencana menggunakan sensor I2C (seperti encoder magnetik AS5600), Anda **harus** memindahkan pin encoder ke pin lain yang mendukung interupsi (misalnya, `PA0`, `PA1`) dan memperbarui definisi `QUAD_ENC_PIN_A` dan `QUAD_ENC_PIN_B` di berkas `QuadEncoder.h`.

---

## Langkah 1: Modifikasi Definisi Papan Arduino

(Langkah-langkah lainnya tetap sama...)

## Langkah 2: Konfigurasi dan Kompilasi

(Langkah-langkah lainnya tetap sama...)

## Langkah 3: Unggah Firmware

(Langkah-langkah lainnya tetap sama...)
