# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

LUFA_PATH     = $(CURDIR)/LUFA/LUFA
COMPILER_PATH = /usr/local/CrossPack-AVR/bin/

MCU      = at90usb1286
ARCH     = AVR8
BOARD    = TEENSY2
F_CPU    = 16000000
F_USB    = 16000000

TARGET   = Flash
SRC      = main.c Descriptors.c Joystick.c $(LUFA_SRC_USB_DEVICE) $(LUFA_SRC_USBCLASS_DEVICE)
CC_FLAGS = -Werror -DUSE_LUFA_CONFIG_HEADER

# Default target
teensy:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
