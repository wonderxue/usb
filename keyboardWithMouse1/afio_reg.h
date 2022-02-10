#pragma once

#define AFIO_BASE 0x40010000

typedef struct
{
    volatile unsigned int EVCR;
    volatile unsigned int MAPR;
    volatile unsigned int EXTICR1;
    volatile unsigned int EXTICR2;
    volatile unsigned int EXTICR3;
    volatile unsigned int EXTICR4;
}AFIO_def;

#define AFIO  ((AFIO_def *)AFIO_BASE)