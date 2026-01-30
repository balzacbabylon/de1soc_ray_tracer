#include "platform.h"
#include <stdio.h>

#define JTAG_UART_BASE 0xFF201000
#define PIXEL_BUF_CTRL 0xFF203020
#define KEY_BASE       0xFF200050

// Hide the buffer inside the .c file so external code can't corrupt it directly
static short int buffer1[240][512];
static short int buffer2[240][512];
static volatile int pixel_buffer_start;
static volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL;

// TODO: Should the error checking for out of bounds be here? 
void put_pixel(int x, int y, short int color) {
    // Coordinate transformation
    if (x > 160 || x < -159 || y > 119 || y < -120) return;
    buffer2[119 - y][159 + x] = color;
}

void platform_init(void) {
    // Set the back buffer address
    *(pixel_ctrl_ptr + 1) = (int) &buffer1;
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
            // Note: buffer index logic must match put_pixel's coordinate system
            // put_pixel uses: buffer[119 - y][159 + x]
            // We can just clear the whole raw array row by row for simplicity
             buffer1[y][x] = 0x0000;
        }
    }
    *(pixel_ctrl_ptr + 1) = (int) &buffer2;
    
}

void platform_swap_buffers(void) {
    *(pixel_ctrl_ptr) = 1; // Trigger buffer swap
}

// Reads the raw value of the pushbutton data register
int platform_read_keys(void) {
    volatile int * key_ptr = (int *)KEY_BASE;
    return *key_ptr;
}

// Clears the buffer to black (0x0000) so the object can move without leaving trails
void platform_clear_screen(void) {
    // We only need to clear the visible 320x240 area to save time
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
            // Note: buffer index logic must match put_pixel's coordinate system
            // put_pixel uses: buffer[119 - y][159 + x]
            // We can just clear the whole raw array row by row for simplicity
             buffer2[y][x] = 0x0000;
        }
    }
}

void wait_for_vsync(){
    int status;

    *pixel_ctrl_ptr = 1;  // start synchronization process
                        // write 1 into front buffer address register

    status = *(pixel_ctrl_ptr + 3);  // read status register

    while ((status & 0x01) != 0) {  // polling loop waiting for S bit to go to 0
        status = *(pixel_ctrl_ptr + 3);
    }
} 

/*
 * Helper: Sends a single character to the JTAG UART.
 * modified to WAIT for space instead of dropping characters.
 */
void put_jtag(volatile int * JTAG_UART_ptr, char c) {
    int control;
    // Poll the control register until write space is available
    // The upper 16 bits (0xFFFF0000) hold the available space count
    do {
        control = *(JTAG_UART_ptr + 1); 
    } while ((control & 0xFFFF0000) == 0);

    *(JTAG_UART_ptr) = c;
}

/*
 * Public Function: Prints a string followed by a newline
 */
void printuart(const char *ptr) {
    volatile int *JTAG_UART_ptr = (int *)JTAG_UART_BASE;

    // Loop through the string until the null terminator
    while (*ptr != '\0') {
        put_jtag(JTAG_UART_ptr, *ptr);
        ptr++;
    }

    // Append the new line
    put_jtag(JTAG_UART_ptr, '\n'); 
}