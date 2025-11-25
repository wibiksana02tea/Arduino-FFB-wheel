#ifndef QuadEncoder_h
#define QuadEncoder_h

#include <Arduino.h>

// --- Pin Definitions for STM32 ---
// These should be defined in your main sketch or Config.h
#ifndef QUAD_ENC_PIN_A
#define QUAD_ENC_PIN_A PB6 // Example pin
#endif
#ifndef QUAD_ENC_PIN_B
#define QUAD_ENC_PIN_B PB7 // Example pin
#endif

class cQuadEncoder
{
public:
  cQuadEncoder();
  void Init(s32 initial_pos = 0, bool use_pullups = true);
  s32 Read();
  void Write(s32 value);
  void processInterrupt();

  static int32_t mPosition;
  static uint8_t mEncoderState;
  static cQuadEncoder * pStaticEncoder;
};

#endif
