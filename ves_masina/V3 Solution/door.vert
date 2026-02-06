#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;

out vec2 vUV;

uniform vec2 uOffset;     // door center in world
uniform vec2 uScale;      // scale : 1.1 beacaouse vao is already in world
uniform float uRotation;  // rad
uniform vec2 uPivot;  

void main()
{
    vec2 p = aPos * uScale;
    vec2 world = p + uOffset;

    float c = cos(uRotation);
    float s = sin(uRotation);

    vec2 d = world - uPivot;
    vec2 r = vec2(d.x*c - d.y*s, d.x*s + d.y*c) + uPivot;

    gl_Position = vec4(r, 0.0, 1.0);
    vUV = aUV;
}
