#include <Update.h>

void updateSetup(App* app)
{
    createSSBOs(app);
    mapBuffers(app);

    lastUpdate = GetTime();

    srand((uint32_t) GetTime());
    pcg32_srandom(rand(), rand());
}

void update(App* app)
{
    double deltaTime = GetTime() - lastUpdate;
    lastUpdate = GetTime();

    float* verts = (float*) app->mappedBuffers[0];
    int16_t* rots = (int16_t*) app->mappedBuffers[1];
    int16_t* buffer_rots = (int16_t*) malloc(app->SSBO_arrays[0].vert_count*sizeof(int16_t));
    mallocCheck(buffer_rots);

    //int32_t i;
    //#pragma omp parallel for
    for (uint32_t i = 0; i < app->SSBO_arrays[0].vert_count; i++)
    {
        vec2 i_pos = {
            verts[i*2],
            verts[i*2+1]
        };

        verts[i*2] = i_pos[0] + (float) (cos(WEIRD_TO_RAD(rots[i])) * BOID_SPEED * deltaTime);
        verts[i*2+1] = i_pos[1] + (float) (sin(WEIRD_TO_RAD(rots[i])) * BOID_SPEED * deltaTime);

        i_pos[0] = verts[i*2];
        i_pos[1] = verts[i*2+1];

        float boids_in_flock = 0.0f;
        float boids_too_close = 0.0f;

        int16_t alignment_angle = 0;
        vec2 cohesion_vector = { 0 };
        vec2 separation_vector = { 0 };
        vec2 out_of_bounds_vector = { 0 };
        boolean is_out_of_bounds = 0;

        if (glm_vec2_norm2(i_pos) > 8000)
        {
            is_out_of_bounds = 1;
            glm_vec2_sub(out_of_bounds_vector, i_pos, out_of_bounds_vector);
        }

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
                boids_in_flock++;

                // Alignment
                alignment_angle += angleDiff(rots[i], rots[j]);

                // Cohesion
                glm_vec2_sub(j_pos, i_pos, cohesion_vector);

                // Seperation
                if (j_dist <= BOID_SIZE*BOID_SIZE*2)
                {
                    boids_too_close++;
                    glm_vec2_sub(i_pos, j_pos, separation_vector);
                }
            }
        }

        int16_t alignment_diff = 0;
        int16_t cohesion_diff = 0;
        int16_t separation_diff = 0;
        int16_t out_of_bounds_diff = 0;

        if (boids_in_flock)
        {
            // Alignment
            alignment_angle /= (int16_t) boids_in_flock;
            alignment_diff = weightedDiff(alignment_angle, ALIGNMENT_WEIGHT, deltaTime);

            // Cohesion
            int16_t cohesion_angle = vectorSumToAngle(cohesion_vector, boids_in_flock);
            cohesion_diff = weightedDiff(angleDiff(rots[i], cohesion_angle), COHESION_WEIGHT, deltaTime);
        }

        // Separation
        if (boids_too_close > 0)
        {
            int16_t separation_angle = vectorSumToAngle(separation_vector, boids_too_close);
            separation_diff = weightedDiff(angleDiff(rots[i], separation_angle), SEPARTATION_WEIGHT, deltaTime);
        }

        if (is_out_of_bounds)
        {
            int16_t out_of_bounds_angle = RAD_TO_WEIRD(atan2(out_of_bounds_vector[1], out_of_bounds_vector[0]));
            cohesion_diff = weightedDiff(angleDiff(rots[i], out_of_bounds_angle), SEPARTATION_WEIGHT, deltaTime);
        }

        int16_t avg_diff = (alignment_diff+cohesion_diff+separation_diff+out_of_bounds_diff)/4;
        int16_t new_angle = (rots[i] + avg_diff) % WEIRD_ANGLE_UNIT;

        buffer_rots[i] = new_angle;
    };

    memcpy(rots, buffer_rots, app->SSBO_arrays[0].vert_count*sizeof(int16_t));

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

        vcies[i*2] = 0.0f;
        vcies[i*2+1] = 0.0f;
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
