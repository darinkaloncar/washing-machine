#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

uniform vec2 uOffset;
uniform float uRotation; 
uniform vec2 uScale;

out vec2 vUV;

void main()
{
    // scale
    vec2 p = aPos * uScale;

    // rotate
    float c = cos(uRotation);
    float s = sin(uRotation);
    p = vec2(c*p.x - s*p.y, s*p.x + c*p.y);

    // translate
    p += uOffset;

    gl_Position = vec4(p, 0.0, 1.0);
    vUV = aUV;
}
