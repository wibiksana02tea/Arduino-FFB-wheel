#ifndef ffb_pro_h
#define ffb_pro_h

// FFB Report IDs
#define FFB_ID_SETEFFECT				0x01
#define FFB_ID_ENVELOPE				0x02
#define FFB_ID_CONDITION			0x03
#define FFB_ID_PERIODIC				0x04
#define FFB_ID_CONSTANT				0x05
#define FFB_ID_RAMP					0x06
#define FFB_ID_CUSTOMFORCE			0x07
#define FFB_ID_DOWNLOADFORCESAMPLE	0x08
#define FFB_ID_EFFECTOPERATION		0x0a
#define FFB_ID_BLOCKFREE			0x0b
#define FFB_ID_DEVICECONTROL		0x0c
#define FFB_ID_DEVICEGAIN			0x0d
#define FFB_ID_SETCUSTOMFORCE		0x0e
#define FFB_ID_CREATENEWEFFECT		0x0f
#define FFB_ID_BLOCKLOAD			0x10
#define FFB_ID_POOLREPORT			0x11

// FFB Effect types
#define USB_EFFECT_CONSTANT		0x01
#define USB_EFFECT_RAMP			0x02
#define USB_EFFECT_SQUARE		0x03
#define USB_EFFECT_SINE			0x04
#define USB_EFFECT_TRIANGLE		0x05
#define USB_EFFECT_SAWTOOTHDOWN	0x06
#define USB_EFFECT_SAWTOOTHUP	0x07
#define USB_EFFECT_SPRING		0x08
#define USB_EFFECT_DAMPER		0x09
#define USB_EFFECT_INERTIA		0x0a
#define USB_EFFECT_FRICTION		0x0b
#define USB_EFFECT_CUSTOM		0x0c

// Platform-agnostic includes and definitions
#include <Arduino.h>

void FfbSetDriver(u8 driver);

class cFFB
{
public:
  void FfbOnUsbData(u8 *data, u16 len);
  s32v CalcTorqueCommands(s32v *axis);
};

typedef struct {
  u16 버튼;
  u8 hat;
  u16 x;
  u16 y;
  u16 z;
  u16 rx;
  u16 ry;
  u16 rz;
} tJoystickReport;

#endif
