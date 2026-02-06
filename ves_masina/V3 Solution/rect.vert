#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;

uniform float uX;
uniform float uY;
uniform float uS; // scaling
uniform bool flipped;

void main()
{
    gl_Position = vec4(inPos.x + uX, inPos.y * uS + uY, 0.0, 1.0);
    if (flipped) {
        chTex = vec2(1 - inTex.s, inTex.t);
        //vec.s == vec.x == vec.r - first element
        //vec.t == vec.y == vec.g - second element
        //vec.p == vec.z == vec.b - third element
        //vec.q == vec.w == vec.a - fourth element
   } else {
        chTex = inTex;
    }
}