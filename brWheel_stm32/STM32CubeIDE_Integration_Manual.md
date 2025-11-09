# Manual Integrasi STM32CubeIDE untuk Firmware Roda Umpan Balik Gaya

Panduan ini menjelaskan cara mengintegrasikan kode firmware `brWheel_stm32` yang telah di-porting ke dalam proyek STM32CubeIDE yang dihasilkan. Ini mengasumsikan Anda memiliki pemahaman dasar tentang STM32CubeIDE dan mikrokontroler STM32.

## Langkah 1: Buat dan Konfigurasikan Proyek di STM32CubeIDE

1.  **Buat Proyek Baru**:
    *   Buka STM32CubeIDE dan buka `File > New > STM32 Project`.
    *   Pilih mikrokontroler STM32 target Anda (misalnya, STM32F401RE).
    *   Beri nama proyek Anda (misalnya, `brWheel_Firmware`) dan pilih "STM32Cube" sebagai Tipe Proyek. Klik "Finish".
    *   Ketika ditanya, inisialisasi semua periferal dengan mode default mereka.

2.  **Konfigurasi Sistem Clock**:
    *   Di tab "Pinout & Configuration", buka `System Core > RCC`.
    *   Untuk `High Speed Clock (HSE)`, pilih `Crystal/Ceramic Resonator`.
    *   Buka tab "Clock Configuration" dan atur `HCLK` ke nilai maksimum yang diizinkan untuk perangkat Anda (misalnya, 84 MHz). CubeMX akan menyelesaikan pohon clock secara otomatis. Pastikan clock USB (jika berlaku) diatur ke 48MHz.

3.  **Konfigurasi Periferal yang Diperlukan**:
    *   **Timer untuk `micros()`**:
        *   Buka `Timers > TIM2` (atau timer 32-bit lainnya).
        *   Atur `Clock Source` ke `Internal Clock`.
        *   Atur `Prescaler` agar frekuensi clock timer menjadi 1MHz (misalnya, jika APB1 Timer Clocks adalah 84MHz, atur Prescaler ke `83`). Ini akan menghasilkan tick 1-mikrodetik.
    *   **Timer untuk PWM**:
        *   Buka `Timers > TIM1` (atau timer canggih lainnya).
        *   Atur `Clock Source` ke `Internal Clock`.
        *   Atur `Channel1`, `Channel2`, `Channel3`, `Channel4` ke `PWM Generation CHx`.
        *   Di bawah "Parameter Settings", atur `Counter Period` (ARR) ke nilai yang ditentukan oleh `MAX_PWM_RESOLUTION` di `Config_STM32.h` (misalnya, `4095` untuk resolusi 12-bit).
    *   **ADC**:
        *   Buka `Analog > ADC1`.
        *   Aktifkan semua saluran yang sesuai dengan pin analog Anda (misalnya, `IN0` untuk `PA0`).
        *   Di bawah "Parameter Settings", pastikan `Resolution` diatur ke `12 bits`.
    *   **I2C**:
        *   Buka `Connectivity > I2C1`.
        *   Atur `Mode` ke `I2C`.
    *   **USB**:
        *   Buka `Connectivity > USB` (atau `USB_OTG_FS`).
        *   Atur `Mode` ke `Device (FS)`.
        *   Di `Middleware`, klik `USB_DEVICE`.
        *   Atur `Class for FS IP` ke `Human Interface Device Class (HID)`.
    *   **GPIO**:
        *   Untuk setiap pin yang akan Anda gunakan untuk tombol, atur ke mode `GPIO_Input`. Beri mereka nama label yang mudah dikenali (misalnya, `BTN_0`).

4.  **Hasilkan Kode**:
    *   Simpan berkas `.ioc`. CubeIDE akan meminta untuk menghasilkan kode. Klik "Yes".

## Langkah 2: Tambahkan Berkas Firmware yang Telah Di-porting

1.  **Buat Folder Virtual**:
    *   Di Project Explorer, klik kanan pada folder `Src` dan pilih `New > Folder`.
    *   Beri nama folder `brWheel` dan klik "Finish".
2.  **Salin Berkas**:
    *   Salin semua berkas `.cpp` dan `.h` dari direktori `brWheel_stm32` repositori ini ke dalam folder `Src/brWheel` yang baru saja Anda buat di proyek Anda.
3.  **Tambahkan Path ke Build Settings**:
    *   Klik kanan pada proyek Anda dan pilih `Properties`.
    *   Buka `C/C++ General > Paths and Symbols`.
    *   Di tab "Includes", klik `Add...`, pilih `Workspace...`, dan tambahkan path ke folder `Src/brWheel` Anda.
    *   Klik "Apply and Close".

## Langkah 3: Ganti Deskriptor USB HID

Tumpukan USB yang dihasilkan oleh CubeMX memiliki deskriptor HID generik. Anda harus menggantinya dengan deskriptor yang benar untuk roda umpan balik gaya.

1.  **Temukan Deskriptor Asli**: Berkas `USBDesc.h` asli di direktori `brWheel_my` berisi deskriptor HID yang benar. Anda perlu menerjemahkan ini ke dalam format yang digunakan oleh tumpukan USB STM32.
2.  **Edit Deskriptor STM32**:
    *   Buka berkas `usbd_hid.c` (biasanya di `USB_DEVICE/App`).
    *   Temukan array `HID_FOO_ReportDesc` (nama mungkin berbeda).
    *   Ganti konten array ini dengan deskriptor dari `USBDesc.h`. Anda perlu memformatnya sebagai array byte C.
    *   Pastikan untuk memperbarui ukuran deskriptor (`USBD_HID_REPORT_DESC_SIZE`) agar sesuai dengan ukuran deskriptor baru Anda.

## Langkah 4: Hubungkan Firmware ke `main.c`

1.  **Sertakan Header Utama**:
    *   Di bagian atas `main.c`, sertakan header utama untuk firmware: `#include "brWheel/brWheel_stm32.h"`.
2.  **Panggil `setup()`**:
    *   Di `main()` setelah semua inisialisasi `MX_` selesai, tambahkan panggilan ke fungsi setup firmware:
        ```c
        /* USER CODE BEGIN 2 */
        setup();
        /* USER CODE END 2 */
        ```
3.  **Panggil `loop()`**:
    *   Di dalam `while(1)` loop di `main()`, tambahkan panggilan ke fungsi loop utama firmware:
        ```c
        /* USER CODE BEGIN WHILE */
        while (1)
        {
          loop();
        /* USER CODE END WHILE */
        ```

## Langkah 5: Hubungkan Callback FFB

1.  **Temukan Callback USB**: Tumpukan USB STM32 memanggil fungsi callback ketika laporan OUT (data FFB dari host) diterima. Fungsi ini biasanya bernama `USBD_HID_DataIn` atau serupa di `usbd_hid.c`.
2.  **Panggil Fungsi Firmware**: Dari dalam callback USB ini, panggil fungsi yang menangani data FFB di firmware kita:
    ```c
    // Di dalam usbd_hid.c, di dalam fungsi callback penerimaan data...
    // Panggil fungsi dari USBCore_STM32.cpp
    HID_ReceiveReport_Callback(report->data, report->len);
    ```
    Anda mungkin perlu mendeklarasikan `HID_ReceiveReport_Callback` sebagai `extern` di bagian atas `usbd_hid.c`.

## Langkah 6: Bangun dan Flash

1.  **Bangun Proyek**: Klik kanan pada proyek dan pilih `Build Project`.
2.  **Perbaiki Kesalahan**: Anda mungkin mengalami kesalahan kompilasi karena jalur yang hilang atau inkonsistensi kecil. Atasi ini sesuai kebutuhan.
3.  **Flash Firmware**: Setelah proyek berhasil dibangun, gunakan `Run > Run` untuk mem-flash firmware ke papan STM32 Anda.

Perangkat sekarang harus dijumlahkan sebagai joystick umpan balik gaya, dan logika firmware utama akan berjalan di perangkat keras STM32.
