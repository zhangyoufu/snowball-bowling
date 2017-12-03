#include "Joystick.h"
#include <LUFA/Drivers/USB/USB.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdbool.h>

int main(void) {
    clock_prescale_set(clock_div_1);
    sei();
    USB_Init();
    while (1)
    {
        HID_Device_USBTask(&Joystick_Interface);
        USB_USBTask();
    }
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
    HID_Device_ConfigureEndpoints(&Joystick_Interface);
}
