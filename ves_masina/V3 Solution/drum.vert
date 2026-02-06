#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

out vec2 chTex;

uniform vec2 uOffset;
uniform float uRotation;
uniform float uAspect;
uniform vec2 uNudge;
uniform float uScale;  

void main()
{
    float c = cos(uRotation);
    float s = sin(uRotation);

    vec2 p = inPos * uScale;

    // rotate
    p = vec2(
        c * p.x - s * p.y,
        s * p.x + c * p.y
    );

    // translate + manual nudge
    vec2 pos = p + uOffset + uNudge;

    pos.x /= uAspect;

    gl_Position = vec4(pos, 0.0, 1.0);
    chTex = inUV;
}
