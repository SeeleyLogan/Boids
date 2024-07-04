#include <Update.h>

void updateSetup(App* app)
{
    createSSBOs(app);
    mapBuffers(app);

    lastUpdate = GetTime();

    srand((uint32_t) time(0));
    pcg32_srandom(rand(), rand());
}

void update(App* app)
{
    double deltaTime = GetTime() - lastUpdate;
    lastUpdate = GetTime();

    float* verts = (float*) app->mappedBuffers[0];
    int16_t* rots = (int16_t*) app->mappedBuffers[1];

    //int32_t i;
    //#pragma omp parallel for
    for (uint32_t i = 0; i < app->SSBO_arrays[0].vert_count; i++)
    {
        vec2 i_pos = {
            verts[i*2],
            verts[i*2+1]
        };

        vec2 buffer_vector = { 0 };
        if (glm_vec2_distance2(i_pos, buffer_vector) > 10000) // Boid outside radius of 100
        {
            verts[i*2] = 0.0f;
            verts[i*2+1] = 0.0f;
        } else {
            verts[i*2] = i_pos[0] + (float) (cos(WEIRD_TO_RAD(rots[i])) * BOID_SPEED * deltaTime);
            verts[i*2+1] = i_pos[1] + (float) (sin(WEIRD_TO_RAD(rots[i])) * BOID_SPEED * deltaTime);
        }

        i_pos[0] = verts[i*2];
        i_pos[1] = verts[i*2+1];

        float boids_in_flock = 0.0f;
        float boids_too_close = 0.0f;

        vec2 alignment_vector = { 0 };
        vec2 cohesion_vector = { 0 };
        vec2 separation_vector = { 0 };

        for (uint32_t j = 0; j < app->SSBO_arrays[0].vert_count; j++)
        {
            if (i == j)
                continue;

            vec2 j_pos = {
                verts[j*2],
                verts[j*2+1]
            };

            float j_dist = glm_vec2_distance2(i_pos, j_pos);
            if (j_dist <= FLOCK_SIZE)
            {
                // Seperation
                if (j_dist <= FLOCK_SIZE/25)
                {
                    boids_too_close++;
                    glm_vec2_add(separation_vector, buffer_vector, separation_vector);
                    continue;
                }

                boids_in_flock++;

                // Alignment
                buffer_vector[0] = (float) cos(WEIRD_TO_RAD(rots[j]));
                buffer_vector[1] = (float) sin(WEIRD_TO_RAD(rots[j]));

                glm_vec2_add(alignment_vector, buffer_vector, alignment_vector);

                // Cohesion
                glm_vec2_sub(i_pos, j_pos, buffer_vector);
                glm_vec2_add(cohesion_vector, buffer_vector, cohesion_vector);
            }
        }

        // Seperation
        int16_t separation_diff = 0;
        if (boids_too_close > 0)
        {
            glm_vec2_divs(separation_vector, boids_too_close, separation_vector);
            glm_vec2_normalize(separation_vector);

            int16_t separation_angle = RAD_TO_WEIRD(atan2(separation_vector[1], separation_vector[0])) + WEIRD_ANGLE_UNIT/2;
            separation_diff = angleDiff(rots[i], separation_angle);

            separation_diff = (int16_t) glm_clamp((float) -separation_diff, -SEPARTATION_WEIGHT, SEPARTATION_WEIGHT);

            rots[i] += separation_diff;
        }
        else if (boids_in_flock > 0)
        {
            // Alignment
            glm_vec2_divs(alignment_vector, boids_in_flock, alignment_vector);
            glm_vec2_normalize(alignment_vector);

            int16_t alignment_angle = RAD_TO_WEIRD(atan2(alignment_vector[1], alignment_vector[0]));
            int16_t alignment_diff = angleDiff(rots[i], alignment_angle);

            alignment_diff = (int16_t) glm_clamp((float) alignment_diff, -ALIGNMENT_WEIGHT, ALIGNMENT_WEIGHT);

            // Cohesion
            glm_vec2_divs(cohesion_vector, boids_in_flock, cohesion_vector);
            glm_vec2_normalize(cohesion_vector);

            int16_t cohesion_angle = RAD_TO_WEIRD(atan2(cohesion_vector[1], cohesion_vector[0]));
            int16_t cohesion_diff = angleDiff(rots[i], cohesion_angle);

            cohesion_diff = (int16_t) glm_clamp((float) -cohesion_diff, -COHESION_WEIGHT, COHESION_WEIGHT);

            int16_t avg_diff = (alignment_diff+cohesion_diff)/3;

            int16_t new_angle = (rots[i] + avg_diff) % WEIRD_ANGLE_UNIT;

            rots[i] = new_angle;
        }
    };

    glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, app->SSBO_arrays[0].SSBO_array[0]) );
    glCheck( glFlushMappedBufferRange(GL_SHADER_STORAGE_BUFFER, 0, app->SSBO_arrays[0].vert_count*sizeof(float)) );
    glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, app->SSBO_arrays[0].SSBO_array[1]) );
    glCheck( glFlushMappedBufferRange(GL_SHADER_STORAGE_BUFFER, 0, app->SSBO_arrays[0].vert_count*sizeof(int16_t)) );
}

void createSSBOs(App* app)
{
    float* vcies = (float*) malloc(BOIDS*2*sizeof(float));
    int16_t* rots = (int16_t*) malloc(BOIDS*sizeof(int16_t));

    mallocCheck(vcies);
    mallocCheck(rots);

    for (uint32_t i=0; i<BOIDS; i++)
    {   
        float x = ((float) (pcg32_random() % 20001) - 10000.0f) / 100.0f;
        float y = ((float) (pcg32_random() % 20001) - 10000.0f) / 100.0f;
        int16_t rot = (int16_t) (pcg32_random() % 32767);

        vcies[i*2] = x;
        vcies[i*2+1] = y;
        rots[i] = rot;
    }

    const void* data[] = {
        vcies,
        rots
    };

    const size_t data_sizes[] = {
        BOIDS*2*sizeof(float),
        BOIDS*sizeof(int16_t)
    };

    SSBO_a points = createSSBO_a(data, data_sizes, len(data), (uint32_t) data_sizes[1]/sizeof(int16_t), GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    arrput(app->SSBO_arrays, points);
}

void mapBuffers(App* app)
{
    // Verticies
    glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, app->SSBO_arrays[0].SSBO_array[0]) );

    float* vertsMap = glCheck( glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, app->SSBO_arrays[0].vert_count*sizeof(float),
        GL_MAP_PERSISTENT_BIT |
        GL_MAP_WRITE_BIT |
        GL_MAP_READ_BIT |
        GL_MAP_FLUSH_EXPLICIT_BIT |
        GL_MAP_COHERENT_BIT
    ));

    arrput(app->mappedBuffers, vertsMap);

    // Rotations
    glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, app->SSBO_arrays[0].SSBO_array[1]) );

    int16_t* rotsMap = glCheck( glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, app->SSBO_arrays[0].vert_count*sizeof(int16_t),
        GL_MAP_PERSISTENT_BIT |
        GL_MAP_WRITE_BIT |
        GL_MAP_READ_BIT |
        GL_MAP_FLUSH_EXPLICIT_BIT |
        GL_MAP_COHERENT_BIT
    ));

    arrput(app->mappedBuffers, rotsMap);

    glCheck( glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0) );
}
