#version 460 core

#define M_PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

layout(std430, binding = 1) buffer ssboRots
{
    uint rots_SSBO[];
};

out vec4 ourColor;

uniform mat4 orangutanProj;
uniform float boidSize;

void main()
{
    vec4 p = gl_in[0].gl_Position;

    // Rotations are shorts, extract them from the int array
    float rot = float((rots_SSBO[gl_PrimitiveIDIn/2] >> (16*(gl_PrimitiveIDIn%2))) & 0xFFFF)/32767.0*M_PI*2.0 - M_PI/2;

    // Z rotation matrix
    mat4 rotProj;
    rotProj[0] = vec4(cos(rot), -sin(rot), 0.0, 0.0);
    rotProj[1] = vec4(sin(rot), cos(rot), 0.0, 0.0);
    rotProj[2] = vec4(0.0, 0.0, 1.0, 0.0);
    rotProj[3] = vec4(0.0, 0.0, 0.0, 1.0);

    // Create triangle
    vec4 color = vec4(sin(rot), sin(rot+M_PI/3*2), sin(rot+M_PI/3*4), 1.0);

    gl_Position = p + vec4(0.0, 1.0*boidSize, 0.0, 0.0) * rotProj * orangutanProj;
    ourColor = color;
    EmitVertex();

    gl_Position = p + vec4(0.643*boidSize, -0.766*boidSize, 0.0, 0.0) * rotProj * orangutanProj;
    ourColor = color;
    EmitVertex();

    gl_Position = p + vec4(-0.643*boidSize, -0.766*boidSize, 0.0, 0.0) * rotProj * orangutanProj;
    ourColor = color;
    EmitVertex();

    EndPrimitive();
}