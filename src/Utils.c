#include <Utils.h>

int16_t vectorSumToAngle(vec2 vector, float scalar)
{
    glm_vec2_divs(vector, scalar, vector);
    glm_vec2_normalize(vector);
    int16_t separation_angle = RAD_TO_WEIRD(atan2(vector[1], vector[0]));

    return separation_angle;
}