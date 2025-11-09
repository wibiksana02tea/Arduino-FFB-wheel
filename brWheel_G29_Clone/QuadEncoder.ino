#include "QuadEncoder.h"

// --- Static variables and ISR handler ---
static int32_t cQuadEncoder::mPosition;
static uint8_t cQuadEncoder::mEncoderState;
static cQuadEncoder * cQuadEncoder::pStaticEncoder;

// This is the platform-agnostic interrupt handler
void updateEncoder() {
    if (cQuadEncoder::pStaticEncoder != NULL) {
        cQuadEncoder::pStaticEncoder->processInterrupt();
    }
}

// --- cQuadEncoder Class Implementation ---

cQuadEncoder::cQuadEncoder() {
    pStaticEncoder = this; // Store this instance for the static ISR
}

void cQuadEncoder::Init(s32 initial_pos, bool use_pullups) {
    mPosition = initial_pos;

    // Configure pins using standard Arduino functions
    uint8_t pin_mode = INPUT;
    if (use_pullups) {
        pin_mode = INPUT_PULLUP;
    }
    pinMode(QUAD_ENC_PIN_A, pin_mode);
    pinMode(QUAD_ENC_PIN_B, pin_mode);

    // Read initial state
    mEncoderState = 0;
    if (digitalRead(QUAD_ENC_PIN_A)) {
        mEncoderState |= 1;
    }
    if (digitalRead(QUAD_ENC_PIN_B)) {
        mEncoderState |= 2;
    }

    // Attach interrupts using standard Arduino API
    // digitalPinToInterrupt() is required to map pin numbers to interrupt numbers
    attachInterrupt(digitalPinToInterrupt(QUAD_ENC_PIN_A), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(QUAD_ENC_PIN_B), updateEncoder, CHANGE);
}

void cQuadEncoder::processInterrupt(void) {
    uint8_t ab = 0;
    if (digitalRead(QUAD_ENC_PIN_A)) ab |= 1;
    if (digitalRead(QUAD_ENC_PIN_B)) ab |= 2;

    uint8_t prevState = mEncoderState;
    mEncoderState = ab;

    // State transition logic (remains the same)
	if ((prevState == 0 && mEncoderState == 1) || (prevState == 1 && mEncoderState == 3) || (prevState == 3 && mEncoderState == 2) || (prevState == 2 && mEncoderState == 0))
		mPosition++;
	else if ((prevState == 0 && mEncoderState == 2) || (prevState == 2 && mEncoderState == 3) || (prevState == 3 && mEncoderState == 1) || (prevState == 1 && mEncoderState == 0))
		mPosition--;
}

s32 cQuadEncoder::Read() {
    return mPosition;
}

void cQuadEncoder::Write(s32 value) {
    mPosition = value;
}
