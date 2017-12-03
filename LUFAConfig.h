#pragma once

/* General USB Driver Related Tokens: */
#define ORDERED_EP_CONFIG
#define USE_STATIC_OPTIONS                  (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
#define USB_DEVICE_ONLY
#define NO_LIMITED_CONTROLLER_CONNECT
#define NO_SOF_EVENTS

/* USB Device Mode Driver Related Tokens: */
#define USE_FLASH_DESCRIPTORS
#define NO_INTERNAL_SERIAL
#define FIXED_CONTROL_ENDPOINT_SIZE         64
#define DEVICE_STATE_AS_GPIOR               2
#define FIXED_NUM_CONFIGURATIONS            1
#define MAX_ENDPOINT_INDEX                  1
#define NO_DEVICE_REMOTE_WAKEUP
#define NO_DEVICE_SELF_POWER
