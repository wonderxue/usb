#pragma once

#include <wchar.h>
#include "debug.h"
#include "usb_cmd.h"
#include "usb_hid.h"
#include "string.h"

#define READ_MAX_BUFFER_SIZE 100
#define WRITE_MAX_BUFFER_SIZE 100

#pragma pack(1)
typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short bcdUSB;
  unsigned char bDeviceClass;
  unsigned char bDeviceSubClass;
  unsigned char bDeviceProtocol;
  unsigned char bMaxPacketSize0;
  unsigned short idVendor;
  unsigned short idProduct;
  unsigned short bcdDevice;
  unsigned char iManufacturer;
  unsigned char iProduct;
  unsigned char iSerialNumber;
  unsigned char bNumConfigurations;
} USB_DeviceDescriptor;
 
typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short wTotalLength;
  unsigned char bNumInterfaces;
  unsigned char bConfigurationValue;
  unsigned char iConfiguration;
  unsigned char bmAttributes;
  unsigned char bMaxPower;
} USB_ConfigurationDescriptor;
 
typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned char bInterfaceNumber;
  unsigned char bAlternateSetting;
  unsigned char bNumEndpoints;
  unsigned char bInterfaceClass;
  unsigned char bInterfaceSubClass;
  unsigned char bInterfaceProtocol;
  unsigned char iInterface;
} USB_InterfaceDescriptor;
 
typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned char bEndpointAddress;
  unsigned char bmAttributes;
  unsigned short wMaxPacketSize;
  unsigned char bInterval;
} USB_EndpointDescriptor;
 
typedef struct
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short wData[0];
} USB_StringDescriptor;

#pragma pack()

typedef struct
{
  unsigned char maxPacketSize;
  USB_DeviceDescriptor *deviceDescriptor;
  USB_ConfigurationDescriptor *configDescriptor;
  USB_StringDescriptor *stringDescriptor;
  HID_ReportDescriptor *reportDescriptor;

  #pragma region //out
  void (*getDeviceDescriptor)(void);
  void (*getConfigurationDescriptor)(void);
  void (*getStringDescriptor)(const unsigned short wValue,const unsigned short wIndex);
  int (*getReportDescriptor)(void);
  #pragma endregion

  #pragma region //in
  void (*setAddress)(unsigned short address);
  #pragma endregion
}USB_EP0R;
typedef struct
{
  unsigned char maxPacketSize;
  unsigned char isAvailable;
  #pragma region //out
  
  #pragma endregion

  #pragma region //in
  void (*sendReport)(unsigned char *buff,int length);
  #pragma endregion
}USB_EP1R;
typedef struct
{
  unsigned short addr;
  unsigned char status;//0-reseted,1-addr get,2-addr set,3-configed
  unsigned char dataToSend;//-1????????????0-??????0???
  unsigned char *pread;
  unsigned char *pwrite;

  USB_EP0R *endpoint0;
  USB_EP1R *endpoint1;

  void (*init)(void);
}USB_App;

extern USB_App app;