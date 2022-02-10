#pragma once

#pragma pack(1)
typedef struct
{
    unsigned char bmRequestType;
    unsigned char bRequest;
    unsigned short wValue;
    unsigned short wIndex;
    unsigned short wLength;
} USB_DeviceRequest;
#pragma pack()

typedef enum _TRANSFER_DIR
{
  OUT_DIR,
  IN_DIR
}TRANSFER_DIR;

typedef enum _REQUEST_TYPE
{
  STANDARD_REQ,
  CLASS_REQ,
  VENDOR_REQ
}REQUEST_TYPE;

typedef enum _RECIPIENT_TYPE
{
  DEVICE_RECIPIENT,     /* Recipient device */
  INTERFACE_RECIPIENT,  /* Recipient interface */
  ENDPOINT_RECIPIENT,   /* Recipient endpoint */
  OTHER_RECIPIENT
} RECIPIENT_TYPE;
 
 
typedef enum _STANDARD_REQUESTS
{
  GET_STATUS = 0,
  CLEAR_FEATURE,
  RESERVED1,
  SET_FEATURE,
  RESERVED2,
  SET_ADDRESS,
  GET_DESCRIPTOR,
  SET_DESCRIPTOR,
  GET_CONFIGURATION,
  SET_CONFIGURATION,
  GET_INTERFACE,
  SET_INTERFACE,
  SYNCH_FRAME = 12
} STANDARD_REQUESTS;

typedef enum _CLASS_REQUESTS
{
  SET_IDLE = 0x0a,

} CLASS_REQUESTS;
 
/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,
  CONFIG_DESCRIPTOR,
  STRING_DESCRIPTOR,
  INTERFACE_DESCRIPTOR,
  ENDPOINT_DESCRIPTOR,
  DEVICE_BOS_DESCRIPTOR = 0xF,
  HID_DESCRIPTOR=0x21,
  REPORT_DESCRIPTOR,
  PHYSICAL_DESCRIPTOR
} DESCRIPTOR_TYPE;
 
/* Feature selector of a SET_FEATURE or CLEAR_FEATURE */
typedef enum _FEATURE_SELECTOR
{
  ENDPOINT_STALL,
  DEVICE_REMOTE_WAKEUP
} FEATURE_SELECTOR;
 

#define TRANSFER_DIR_MASK      0x80
#define REQUEST_TYPE_MASK      0x60
#define RECIPIENT_MASK         0x1F

#define TRANSFER_DIR_BIT      7
#define REQUEST_TYPE_BIT      5
#define RECIPIENT_BIT         0

#define DESCRIPTOR_TYPE_MASK  0xff00
#define DESCRIPTOR_TYPE_BIT   8