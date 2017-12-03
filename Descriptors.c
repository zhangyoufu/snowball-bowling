#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <avr/pgmspace.h>
#include <stdint.h>

const USB_Descriptor_HIDReport_Datatype_t PROGMEM JoystickReport[] = {
    HID_RI_USAGE_PAGE(8,1), // Generic Desktop
    HID_RI_USAGE(8,5),      // Joystick
    HID_RI_COLLECTION(8,1), // Application
        // Buttons
        HID_RI_LOGICAL_MINIMUM(8,0),
        HID_RI_LOGICAL_MAXIMUM(8,1),
        HID_RI_PHYSICAL_MINIMUM(8,0),
        HID_RI_PHYSICAL_MAXIMUM(8,1),
        HID_RI_REPORT_SIZE(8,1),
        HID_RI_REPORT_COUNT(8,16),
        HID_RI_USAGE_PAGE(8,9),
        HID_RI_USAGE_MINIMUM(8,1),
        HID_RI_USAGE_MAXIMUM(8,16),
        HID_RI_INPUT(8,2),
        // HAT Switch
        HID_RI_USAGE_PAGE(8,1),
        HID_RI_LOGICAL_MAXIMUM(8,7),
        HID_RI_PHYSICAL_MAXIMUM(16,315),
        HID_RI_REPORT_SIZE(8,4),
        HID_RI_REPORT_COUNT(8,1),
        HID_RI_UNIT(8,20),
        HID_RI_USAGE(8,57),
        HID_RI_INPUT(8,66),
        HID_RI_UNIT(8,0),
        HID_RI_REPORT_COUNT(8,1),
        HID_RI_INPUT(8,1),
        // Stick
        HID_RI_LOGICAL_MAXIMUM(8,255),
        HID_RI_PHYSICAL_MAXIMUM(8,255),
        HID_RI_USAGE(8,48),
        HID_RI_USAGE(8,49),
        HID_RI_USAGE(8,50),
        HID_RI_USAGE(8,53),
        HID_RI_REPORT_SIZE(8,8),
        HID_RI_REPORT_COUNT(8,4),
        HID_RI_INPUT(8,2),
        // Vendor Specific
        HID_RI_USAGE_PAGE(16,65280),
        HID_RI_USAGE(8,32),
        HID_RI_REPORT_COUNT(8,1),
        HID_RI_INPUT(8,2),
    HID_RI_END_COLLECTION(0),
};

enum {
    STRING_ID_Manufacturer = 1,
    STRING_ID_Product      = 2,
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor = {
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},
    .USBSpecification       = VERSION_BCD(1,1,0),
    .Class                  = USB_CSCP_NoDeviceClass,
    .SubClass               = USB_CSCP_NoDeviceSubclass,
    .Protocol               = USB_CSCP_NoDeviceProtocol,
    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
    .VendorID               = 0x0F0D,
    .ProductID              = 0x0092,
    .ReleaseNumber          = VERSION_BCD(1,0,0),
    .ManufacturerStrIndex   = STRING_ID_Manufacturer,
    .ProductStrIndex        = STRING_ID_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,
    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS,
};

typedef struct {
    USB_Descriptor_Configuration_Header_t Config;
    USB_Descriptor_Interface_t  HID_Interface;
    USB_Descriptor_Endpoint_t   HID_ReportINEndpoint;
    USB_HID_Descriptor_HID_t    HID_Descriptor;
} USB_Descriptor_Configuration_t;

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor = {
    .Config = {
        .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},
        .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
        .TotalInterfaces        = 1,
        .ConfigurationNumber    = 1,
        .ConfigurationStrIndex  = NO_DESCRIPTOR,
        .ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,
        .MaxPowerConsumption    = USB_CONFIG_POWER_MA(500),
    },
    .HID_Interface = {
        .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},
        .InterfaceNumber        = INTERFACE_ID_Joystick,
        .AlternateSetting       = 0,
        .TotalEndpoints         = 1,
        .Class                  = HID_CSCP_HIDClass,
        .SubClass               = HID_CSCP_NonBootSubclass,
        .Protocol               = HID_CSCP_NonBootProtocol,
        .InterfaceStrIndex      = NO_DESCRIPTOR,
    },
    .HID_ReportINEndpoint = {
        .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},
        .EndpointAddress        = JOYSTICK_IN_EPADDR,
        .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
        .EndpointSize           = JOYSTICK_EPSIZE,
        .PollingIntervalMS      = 255,
    },
    .HID_Descriptor = {
        .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},
        .HIDSpec                = VERSION_BCD(1,1,1),
        .CountryCode            = 0,
        .TotalReportDescriptors = 1,
        .HIDReportType          = HID_DTYPE_Report,
        .HIDReportLength        = sizeof(JoystickReport),
    },
};

uint16_t CALLBACK_USB_GetDescriptor(
    const uint16_t wValue,
    const uint16_t wIndex,
    const void** const DescriptorAddress
) {
    const uint8_t DescriptorType   = (wValue >> 8);
    const uint8_t DescriptorNumber = (wValue & 0xFF);

    const void* Address = NULL;
    uint16_t    Size    = NO_DESCRIPTOR;

    switch (DescriptorType)
    {
        case DTYPE_Device:
        {
            Address = &DeviceDescriptor;
            Size    = sizeof(DeviceDescriptor);
            break;
        }
        case DTYPE_Configuration:
        {
            Address = &ConfigurationDescriptor;
            Size    = sizeof(ConfigurationDescriptor);
            break;
        }
        case DTYPE_String:
        {
            switch (DescriptorNumber)
            {
                case STRING_ID_Manufacturer:
                {
                    static const USB_Descriptor_String_t PROGMEM String = USB_STRING_DESCRIPTOR(L"HORI CO.,LTD.");
                    Address = &String;
                    Size    = pgm_read_byte(&String.Header.Size);
                    break;
                }
                case STRING_ID_Product:
                {
                    static const USB_Descriptor_String_t PROGMEM String = USB_STRING_DESCRIPTOR(L"POKKEN CONTROLLER");
                    Address = &String;
                    Size    = pgm_read_byte(&String.Header.Size);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case HID_DTYPE_Report:
        {
            Address = &JoystickReport;
            Size    = sizeof(JoystickReport);
            break;
        }
    }

    *DescriptorAddress = Address;
    return Size;
}
