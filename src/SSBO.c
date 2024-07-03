#include <SSBO.h>

SSBO_a createSSBO_a(const void** data, const size_t* data_sizes, uint8_t data_count, uint32_t vert_count, int flags)
{
    SSBO_a ssbo_a = {
        NULL,
        data_count,
        vert_count
    };

    ssbo_a.SSBO_array = (uint32_t*) malloc(sizeof(void*)*data_count);
    mallocCheck(ssbo_a.SSBO_array);

    for (uint8_t i=0; i<data_count; i++)
    {
        uint32_t SSBO = 0;
        glCheck( glGenBuffers(1, &SSBO) );
        glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO) );
        glCheck( glBufferStorage(GL_SHADER_STORAGE_BUFFER, data_sizes[i], data[i], flags) );
        glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0) );

        ssbo_a.SSBO_array[i] = SSBO;
    }

    return ssbo_a;
}

void bindSSBO_a(SSBO_a* ssbo_a)
{
    for (uint8_t i=0; i<ssbo_a->SSBO_count; i++)
    {
        glCheck( glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, ssbo_a->SSBO_array[i]) );
    }
}