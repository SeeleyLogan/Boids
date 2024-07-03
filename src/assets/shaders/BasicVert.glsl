#version 460 core

uniform float aspectRatio;

layout(std430, binding = 0) buffer ssboVerts
{
    vec2 verts_SSBO[];
};

layout(std430, binding = 1) buffer ssboColors
{
    vec4 colors_SSBO[];
};

out vec4 ourColor;

void main()
{
    int i = gl_VertexID;

    vec2 aPos = vec2(verts_SSBO[i].x/aspectRatio, verts_SSBO[i].y);

    ourColor = colors_SSBO[i]/255.0;
    gl_Position = vec4(aPos, 0.0, 1.0);
} 