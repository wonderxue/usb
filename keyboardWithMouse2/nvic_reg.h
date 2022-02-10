#pragma once

#define NVIC_BASE  0xe000e100

typedef struct
{
    volatile unsigned int ISER[3];
    unsigned int RESERVED0[29];
    volatile unsigned int ICER[3];
    unsigned int RESERVED1[29];
    volatile unsigned int ISPR[3];
    unsigned int RESERVED2[29];
    volatile unsigned int ICPR[3];
    unsigned int RESERVED3[29];
    volatile unsigned int IABR[3];
    unsigned int RESERVED4[29];
    volatile unsigned char IPR[80];
    unsigned int RESERVED5[685];
    volatile unsigned int STIR;
}NVIC_def;

#define NVIC  ((NVIC_def *)NVIC_BASE)

