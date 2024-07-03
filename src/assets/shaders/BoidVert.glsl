#version 460 core

layout(std430, binding = 0) buffer ssboVerts
{
    vec2 verts_SSBO[];
};

uniform mat4 orangutanProj;

void main()
{
    int i = gl_VertexID;

    gl_Position = vec4(verts_SSBO[i].x, verts_SSBO[i].y, -float(gl_VertexID)/100000000, 1.0) * orangutanProj;
}