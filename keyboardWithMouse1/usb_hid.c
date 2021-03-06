#include "usb_hid.h"

unsigned char ReportDescriptor[REPORTDESCRIPTOR_SIZE] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)

    0x85,0x01,

    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x08,                    //   REPORT_SIZE (1)
    0x75, 0x01,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (5)
    0x75, 0x08,                    //   REPORT_SIZE (1)
    0x15, 0x00,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x25, 0xff,                    //   USAGE_MAXIMUM (Kana)
    0x05, 0x07,                    //   OUTPUT (Data,Var,Abs)
    0x19, 0x00,                    //   REPORT_COUNT (1)
    0x29, 0x65,                    //   REPORT_SIZE (3)
    0x81, 0x00,                    //   OUTPUT (Cnst,Var,Abs)
    0x25, 0x01,                    //   REPORT_COUNT (6)
    0x95, 0x05,                    //   REPORT_SIZE (8)
    0x75, 0x01,                    //   LOGICAL_MINIMUM (0)
    0x05, 0x08,                    //   LOGICAL_MAXIMUM (101)
    0x19, 0x01,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x05,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x91, 0x02,                    //   INPUT (Data,Ary,Abs)
    0x95,0x01,
    0x75,0x03,
    0x91,0x03,
    0xc0,

    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)

    0x85,0x02,
    
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,
    0xc0                           // END_COLLECTION
};

