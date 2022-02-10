#include "debug.h"

void dumpBuffer(const void *buffer,unsigned short length)
{
    const unsigned char *p=buffer;
    while(length--)
    {
        printf(" %02x",*p++);
    }
    printf("\n");
}