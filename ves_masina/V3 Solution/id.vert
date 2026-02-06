#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

out vec2 chTex;

uniform vec2 uOffset;
uniform float uRotation;

void main()
{
    float c = cos(uRotation);
    float s = sin(uRotation);

    vec2 p = vec2(
        c * inPos.x - s * inPos.y,
        s * inPos.x + c * inPos.y
    );

    vec2 pos = p + uOffset;
    gl_Position = vec4(pos, 0.0, 1.0);
    chTex = inUV;
}
