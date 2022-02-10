#pragma once
#define REPORTDESCRIPTOR_SIZE_KEYBOARD 69
#define REPORTDESCRIPTOR_SIZE_MOUSE 54
#pragma pack(1)
 typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short bcdHID;
  unsigned char bCountryCode;
  unsigned char bNumDescriptors;
  unsigned char bDescriptorType0;
  unsigned short wDescriptorLength0;
  #ifdef OPTIONAL_EN
  unsigned char bDescriptorType1;
  unsigned short wDescriptorLength1;
  #endif
}USB_HIDDescriptor;

typedef struct
{
    unsigned char pre;
    unsigned char data;
}_HID_ITEM;

typedef struct
{
    _HID_ITEM item[0];
}HID_ReportDescriptor;


#pragma pack()

