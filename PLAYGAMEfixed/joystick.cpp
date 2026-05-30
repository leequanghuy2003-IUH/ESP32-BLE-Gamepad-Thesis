/*************************************  DO AN TOT NGHIEP
 * ****************************************/
/****************************** THIET BI CHOI GAME QUA BLUETOOTH
 * *********************************/
/**************************************  LE QUANG HUY
 * *******************************************/
/**********************************  NGUYEN LE THANH HIEU
 * ***************************************/

#include "lib.h"

namespace main {
namespace _JOY {

int value1 = 0, value2 = 0, value3 = 0, value4 = 0;

// ── Helpers ───────────────────────────────────────────────────────────────
/**
 * Apply dead zone around the joystick centre.
 * Returns a value in [0, 4095] with a flat centre region.
 */
static int applyDeadZone(int raw) {
  int offset = raw - JOYSTICK_CENTER;
  if (abs(offset) < JOYSTICK_DEAD_ZONE)
    return JOYSTICK_CENTER;

  // Re-map outside dead zone so full HID range is still reachable
  if (offset > 0) {
    return map(offset, JOYSTICK_DEAD_ZONE, 4095 - JOYSTICK_CENTER,
               JOYSTICK_CENTER, 4095);
  } else {
    return map(offset, -JOYSTICK_CENTER, -JOYSTICK_DEAD_ZONE, 0,
               JOYSTICK_CENTER);
  }
}

// ── Init ──────────────────────────────────────────────────────────────────
void init() {
  pinMode(L_VRY_JOYSTICK, INPUT);
  pinMode(L_VRX_JOYSTICK, INPUT);
  pinMode(R_VRY_JOYSTICK, INPUT);
  pinMode(R_VRX_JOYSTICK, INPUT);
}

// ── Main ──────────────────────────────────────────────────────────────────
void main() {
  // Read raw and apply dead zone
  int ly = applyDeadZone(analogRead(L_VRY_JOYSTICK));
  int lx = applyDeadZone(analogRead(L_VRX_JOYSTICK));
  int ry = applyDeadZone(analogRead(R_VRY_JOYSTICK));
  int rx = applyDeadZone(analogRead(R_VRX_JOYSTICK));

  // Map to HID range (0-32737)
  // Y-axes are inverted so "up" = higher value
  value1 = map(ly, 0, 4095, 32737, 0);
  value2 = map(lx, 0, 4095, 0, 32737);
  value3 = map(ry, 0, 4095, 32737, 0);
  value4 = map(rx, 0, 4095, 0, 32737);
}

} // namespace _JOY
} // namespace main