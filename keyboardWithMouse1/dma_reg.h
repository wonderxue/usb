#pragma once
#define DMA_BASE    0x40020000

typedef struct
{
    volatile unsigned int ISR;
    volatile unsigned int IFCR;

    volatile unsigned int CCR1;
    volatile unsigned int CNDTR1;
    volatile unsigned int CPAR1;
    volatile unsigned int CMAR1;
    unsigned int RESERVED1;

    volatile unsigned int CCR2;
    volatile unsigned int CNDTR2;
    volatile unsigned int CPAR2;
    volatile unsigned int CMAR2;
    unsigned int RESERVED2;

    volatile unsigned int CCR3;
    volatile unsigned int CNDTR3;
    volatile unsigned int CPAR3;
    volatile unsigned int CMAR3;
    unsigned int RESERVED3;

    volatile unsigned int CCR4;
    volatile unsigned int CNDTR4;
    volatile unsigned int CPAR4;
    volatile unsigned int CMAR4;
    unsigned int RESERVED4;

    volatile unsigned int CCR5;
    volatile unsigned int CNDTR5;
    volatile unsigned int CPAR5;
    volatile unsigned int CMAR5;
    unsigned int RESERVED5;

    volatile unsigned int CCR6;
    volatile unsigned int CNDTR6;
    volatile unsigned int CPAR6;
    volatile unsigned int CMAR6;
    unsigned int RESERVED6;

    volatile unsigned int CCR7;
    volatile unsigned int CNDTR7;
    volatile unsigned int CPAR7;
    volatile unsigned int CMAR7;
    unsigned int RESERVED7;
}DMA_Def;

#define DMA  ((DMA_Def *)DMA_BASE)
