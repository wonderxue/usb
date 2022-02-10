#pragma once

#include "dma_reg.h"
#include "rcc_reg.h"
#include "usart_reg.h"
#include "nvic_reg.h"

#define DMA_START   DMA->CCR4|=1
#define DMA_STOP    DMA->CCR4&=~1

extern unsigned char usartWrite[1000];
extern unsigned char pstart;
extern unsigned char pstop;
extern unsigned char dataCount;
extern unsigned char isDmaAvailable;
void dmaInit();
void dmaSetNum(unsigned short num);
void dmaSetAddr(unsigned int addr);
