#ifndef SSBO_H
#define SSBO_H

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <Macros.h>

typedef struct SSBO_a
{
    uint32_t* SSBO_array;
    uint8_t SSBO_count;
    uint32_t vert_count;
} SSBO_a;

SSBO_a createSSBO_a(const void** data, const size_t* data_sizes, uint8_t data_count, uint32_t vert_count, int flags);
void bindSSBO_a(SSBO_a* ssbo_a);

#endif