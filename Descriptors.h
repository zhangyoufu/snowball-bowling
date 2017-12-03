#pragma once

#include <LUFA/Drivers/USB/USB.h>

#define JOYSTICK_IN_EPADDR          (ENDPOINT_DIR_IN  | 1)
#define JOYSTICK_EPSIZE             64

enum {
    INTERFACE_ID_Joystick = 0,
};
