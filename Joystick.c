#include "Joystick.h"
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    Y       = 0x01,
    B       = 0x02,
    A       = 0x04,
    X       = 0x08,
    L       = 0x10,
    R       = 0x20,
    ZL      = 0x40,
    ZR      = 0x80,
    MINUS   = 0x100,
    PLUS    = 0x200,
    LCLICK  = 0x400,
    RCLICK  = 0x800,
    HOME    = 0x1000,
    CAPTURE = 0x2000,
} Button_t;

typedef enum {
    HAT_TOP          = 0x00,
    HAT_TOP_RIGHT    = 0x01,
    HAT_RIGHT        = 0x02,
    HAT_BOTTOM_RIGHT = 0x03,
    HAT_BOTTOM       = 0x04,
    HAT_BOTTOM_LEFT  = 0x05,
    HAT_LEFT         = 0x06,
    HAT_TOP_LEFT     = 0x07,
    HAT_CENTER       = 0x08,
} Hat_t;

typedef struct __attribute__((packed)) {
    uint16_t Button;
    uint8_t  Hat;
    uint8_t  LX, LY, RX, RY;
    uint8_t  VendorSpec;
} Report_t;

static Report_t PrevReport;

USB_ClassInfo_HID_Device_t Joystick_Interface = {
    .Config = {
        .InterfaceNumber        = INTERFACE_ID_Joystick,
        .ReportINEndpoint       = {
            .Address = JOYSTICK_IN_EPADDR,
            .Size    = JOYSTICK_EPSIZE,
            .Banks   = 1,
        },
        .PrevReportINBuffer     = &PrevReport,
        .PrevReportINBufferSize = sizeof PrevReport,
    },
};

typedef struct {
    enum {
        ACTION_NONE,
        ACTION_BUTTON,
        ACTION_MOVE,
        ACTION_VIEW,
    } Type;
    union {
        uint16_t Button;
        struct {
            uint8_t X, Y;
        };
        uint16_t Index;
    };
    uint16_t Repeat;
} Action_t;

#define NOTHING     .Type = ACTION_NONE
#define BUTTON(x)   .Type = ACTION_BUTTON, .Button = (x)
#define MOVE(x,y)   .Type = ACTION_MOVE, .X = (x), .Y = (y)
#define VIEW(x,y)   .Type = ACTION_VIEW, .X = (x), .Y = (y)
#define REPEAT(x)   .Repeat = (x)
#define DELAY(x)    NOTHING, REPEAT(x)

static uint8_t Index = 0;
static uint16_t Repeat = 0;

#define Finish()    _Finish(Report, Sequence, sizeof (Sequence) / sizeof (Sequence)[0])
static bool _Finish(Report_t *Report, const Action_t *Sequence, uint16_t SequenceSize)
{
    const Action_t *Action = &Sequence[Index];

    // dispatch
    switch (Action->Type)
    {
        case ACTION_BUTTON:
            Report->Button = Action->Button;
            break;
        case ACTION_MOVE:
            Report->LX = Action->X;
            Report->LY = Action->Y;
            break;
        case ACTION_VIEW:
            Report->RX = Action->X;
            Report->RY = Action->Y;
            break;
        default:
            break;
    }

    // advance
    if (++Repeat >= Action->Repeat)
    {
        Repeat = 0;
        if (++Index >= SequenceSize)
        {
            Index = 0;
            return true;
        }
    }
    return false;
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const _unused,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
    Report_t* Report = (Report_t*)ReportData;
    *ReportSize = sizeof(*Report);

    // initialize report
    Report->Button = 0;
    Report->Hat = HAT_CENTER;
    Report->LX = 0x80;
    Report->LY = 0x80;
    Report->RX = 0x80;
    Report->RY = 0x80;
    Report->VendorSpec = 0;

    // state machine
    static enum {
        INIT,
        TEST,
        DONE,
    } State = INIT;

    switch (State)
    {
        case INIT:
        {
            static const Action_t Sequence[] = {
                { BUTTON(B)   },    // (new controller)
                { NOTHING     },
                { BUTTON(B)   },
                { DELAY(1000) },    // (UI pop-up)
                { BUTTON(B)   },    // (select controller)
                { NOTHING     },
                { BUTTON(A)   },    // Press A when you're ready
                { DELAY(1200) },    // (return to game)
            };
            if (Finish())
            {
                State = TEST;
            }
            break;
        }
        case TEST:
        {
            static const Action_t Sequence[] = {
                { BUTTON(A)   },    // Talk
                { DELAY( 421) },
                { BUTTON(B)   },    // (skip) Hallooooo, kiddums! If you wanted to try a round of Snowball Bowling, then you're in luck, because this is the place!
                { DELAY( 385) },
                { BUTTON(A)   },    // I'll try it!
                { DELAY( 257) },
                { BUTTON(B)   },    // (skip) Ah, the sweet music of "yes"! There's a catch, though. One game costs a cool 20 rupees to play. Still up for a round?
                { DELAY( 385) },
                { BUTTON(A)   },    // OK!
                { DELAY( 257) },
                { BUTTON(B)   },    // (skip) Aha! Play bells are ringing! I gotta set up the pins, but I'll be back in a flurry.
                { BUTTON(A)   },    // (continue)
                { DELAY(7900) },    // (loading)
                { BUTTON(B)   },    // (skip) This is the snowball you'll use. It might not look like much, but it's snow joke. You get two rolls. Knock 'em flat!
                { BUTTON(A)   },    // (continue)
                { DELAY(1153) },
                { BUTTON(B)   },    // (skip) If you can knock over all 10 pins in one roll, that's a strike. Only the most snowtorious bowlers can pull that off!
                { BUTTON(A)   },    // (continue)
                { DELAY( 257) },
                { BUTTON(B)   },    // (skip) A spare is when it takes both tries to finish the job. Not as good as a strike, but nothing to sneeze at!
                { BUTTON(A)   },    // (continue)
                { DELAY( 385) },
                { BUTTON(B)   },    // (skip) Well, good luck!
                { BUTTON(A)   },    // (continue)
                { DELAY( 500) },

                { MOVE(0x00,0x80), REPEAT(200) },
                { DELAY(500) },
                { BUTTON(A)   },    // Pick up
                { DELAY(1000) },
                { MOVE(0xA0,0x00), REPEAT(1500) },
                { MOVE(0x87,0x00), REPEAT( 500) },
                // { MOVE(0xFF,0x00), REPEAT( 500) },  // always fail
                // { VIEW(0x80,0xFF), REPEAT( 500) }, // DEBUG
                { BUTTON(A)   },    // Put down

                { DELAY(15000) },
                { BUTTON(A)    },
                { DELAY(1000)  },
                { BUTTON(A)    },
                { DELAY(10000) },
                { BUTTON(A)    },

                { DELAY(1000) },
                { MOVE(0xFF,0x90), REPEAT(1200) },  // move to talk to Pondo
                { BUTTON(A)   },    // ack / talk
                { DELAY(4000) },
                { BUTTON(A)   },    // question(A)
                { DELAY(5000) },
                { BUTTON(B)   },    // question(B) / ack
                { DELAY(4000) },
                { BUTTON(A)   },    // ack / question(A)
                { DELAY(8000) },
            };
            if (Finish())
            {
                // State = DONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return false;
}
