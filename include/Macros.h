#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>

#include <Globals.h>

#define println(...) printf(__VA_ARGS__);printf("\n");
#define len(x) (sizeof(x)/sizeof(x[0]))
#define angleDiff(a1, a2) ((a2 - a1 + WEIRD_ANGLE_UNIT/2) % WEIRD_ANGLE_UNIT - WEIRD_ANGLE_UNIT/2)
#define RAD_TO_WEIRD(rad) (int16_t) (rad * ((WEIRD_ANGLE_UNIT/2)/M_PI))
#define WEIRD_TO_RAD(weird) (double) weird * (M_PI/(WEIRD_ANGLE_UNIT/2))

#ifdef _DEBUG
#define glCheck(...) \
    __VA_ARGS__; \
    do { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
    { \
        println("error: OpenGL %08x, at %s:%i - for %s\n", err, __FILE__, __LINE__, #__VA_ARGS__); \
        exit(-1); \
    } \
    } while (0)
#else
    #define glCheck(gl_func) gl_func
#endif

#define mallocCheck(ptr) \
    if (ptr == NULL) \
    { \
        println("error: Memory allocation failed"); \
        exit(1); \
    }

#endif