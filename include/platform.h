#ifndef PLATFORM_H
#define PLATFORM_H

#include "common.h"

void platform_init(void);
void platform_swap_buffers(void);
void put_pixel(int x, int y, short int color);

#endif