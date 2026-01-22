#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

// Constants
#define CW  320
#define CH  240
#define FOCAL_LENGTH 256

// Typedefs
typedef int32_t fixed;

typedef struct { 
    int x; 
    int y; 
} Point2D;

typedef struct {
    int x;
    int y;
    fixed h;
} Point;

typedef struct {
    fixed x;
    fixed y;
    fixed z;
} Vertex;

// Array wrappers (Consider renaming to DynamicIntArray etc.)
typedef struct {
    int* data;
    size_t length;
} IntArray;

typedef struct {
    float* data;
    size_t length;
} FArray;

typedef struct {
    fixed* data;
    size_t length;
} FixedArray;

#endif // COMMON_H