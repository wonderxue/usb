#pragma once
#include <stdio.h>

//#define DEBUG_EN

#ifdef DEBUG_EN
#define DEBUG    printf
#define DUMP_BUFFER(x,y) dumpBuffer(x,y)
#else
#define DEBUG
#define DUMP_BUFFER(x,y)
#endif

void dumpBuffer(const void *buffer,unsigned short length);
