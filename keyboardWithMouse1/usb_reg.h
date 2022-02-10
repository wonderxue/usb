#pragma once

#define USB_BASE 0x40005c00
#define USB_PMA  0x40006000
typedef struct
{
    volatile unsigned short EP0R;
    unsigned short RESERVED0;
    volatile unsigned short EP1R;
    unsigned short RESERVED1;
    volatile unsigned short EP2R;
    unsigned short RESERVED2;
    volatile unsigned short EP3R;
    unsigned short RESERVED3;
    volatile unsigned short EP4R;
    unsigned short RESERVED4;
    volatile unsigned short EP5R;
    unsigned short RESERVED5;
    volatile unsigned short EP6R;
    unsigned short RESERVED6;
    volatile unsigned short EP7R;

    unsigned short RESERVED7[17];

    volatile unsigned short CNTR;
    unsigned short RESERVED8;
    volatile unsigned short ISTR;
    unsigned short RESERVED9;
    volatile unsigned short FNR;
    unsigned short RESERVEDA;
    volatile unsigned short DADDR;
    unsigned short RESERVEDB;
    volatile unsigned short BTABLE;
    unsigned short RESERVEDC;
}USB_def;

typedef struct
{
    volatile unsigned short ADDR_TX;
    unsigned short RESERVED0;
    volatile unsigned short COUNT_TX;
    unsigned short RESERVED1;
    volatile unsigned short ADDR_RX;
    unsigned short RESERVED2;
    volatile unsigned short COUNT_RX;
    unsigned short RESERVED3;
}USB_BufferDescriptor_def;


typedef enum
{
    DISABLED=0,
    STALL,
    NAK,
    VALID
}STAT_RTX;

typedef enum
{
    BULK=0,
    CONTROL,
    ISO,
    INTERRUPT
}EP_TYPE;

typedef enum
{
    DBL_BUF=0,
    STATUS_OUT
}EP_KIND;

typedef enum
{
    EP0R=0,
    EP1R,
    EP2R,
    EP3R,
    EP4R,
    EP5R,
    EP6R,
    EP7R,
}EP_NAME;


#define USB  ((USB_def *)USB_BASE)
#define USB_BufferDescriptor ((USB_BufferDescriptor_def *)(USB_PMA+2*USB->BTABLE))

#define CLR_CTR_RX(x)   (x)&=0x0f8f
#define CLR_CTR_TX(x)   (x)&=0x8f0f
#define SET_STAT_RX(x,y)  (x)=((x)&0xbf8f)^((y)<<12)
#define SET_STAT_TX(x,y)  (x)=((x)&0x8fbf)^((y)<<4)
#define SET_EP_TYPE(x,y)  (x)=((x)&0x898f)+((y)<<9)
#define SET_EP_KIND(x,y)  (x)=((x)&0x8e8f)+((y)<<8)
#define GET_EPxR(num)     *((volatile unsigned short*)(USB_BASE+num*4))
