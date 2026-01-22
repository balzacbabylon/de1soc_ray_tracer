#include "platform.h"
#include <stdio.h>

#define JTAG_UART_BASE 0xFF201000
#define PIXEL_BUF_CTRL 0xFF203020

// Hide the buffer inside the .c file so external code can't corrupt it directly
static short int buffer[240][512]; 
static volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL;

// TODO: Should the error checking for out of bounds be here? 
void put_pixel(int x, int y, short int color) {
    // Coordinate transformation
    if (x > 160 || x < -159 || y > 119 || y < -120) return;
    buffer[119 - y][159 + x] = color;
}

void platform_init(void) {
    // Set the back buffer address
    *(pixel_ctrl_ptr + 1) = (int)&buffer;
}

void platform_swap_buffers(void) {
    *(pixel_ctrl_ptr) = 1; // Trigger buffer swap
}