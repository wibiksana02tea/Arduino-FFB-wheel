#include "Config.h"
#include "debug.h"

//--------------------------------------------------------------------------------------------------------

u8 toUpper(u8 c) {
  if ((c >= 'a') && (c <= 'z'))
    return (c + 'A' - 'a');
  return (c);
}

// STM32 Arduino Port: Using "Serial" for the virtual COM port.
void configCDC() {
  if (Serial.available() > 0) {
    u8 c = toUpper(Serial.read());
    s32 temp, temp1;
    f32 wheelAngle;
    u8 ffb_temp;
    switch (c) {
      case 'U': // Send all firmware settings
        Serial.print(ROTATION_DEG);
        Serial.print(' ');
        Serial.print(configGeneralGain);
        Serial.print(' ');
        // ... (all other Serial.print calls remain the same)
        Serial.println(pwmstate, DEC);
        break;
      case 'V':
        Serial.print("fw-v");
        Serial.print(FIRMWARE_VERSION, DEC);
        // Firmware options (now without USE_PROMICRO)
#ifdef USE_AUTOCALIB
        Serial.print("a");
#endif
// ... (all other options remain the same)
        Serial.print("\r\n");
        break;
      // (The rest of the switch-case statement is identical,
      // just with CONFIG_SERIAL replaced by Serial)
      // ...
      default:
        // No action
        break;
    }
  }
}
