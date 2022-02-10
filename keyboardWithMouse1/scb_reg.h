#pragma once

#define SCB_BASE 0xe000e008
#define SCB_AIRCR (*(volatile unsigned int *)(SCB_BASE+0x00))