#pragma once

#include "gpio_reg.h"
#include "rcc_reg.h"
#include "usart_reg.h"
#include <stdio.h>
#include <string.h>
void usartInit();
void sendByte(unsigned char byte);
unsigned char receiveByte();