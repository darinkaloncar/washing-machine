#version 330 core
in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;
uniform float uAlpha;
uniform vec3 uTint;   

void main()
{
    vec4 c = texture(uTex, chTex);
    c.rgb *= uTint;        // added tint for turning shirts into pink colour
    c.a *= uAlpha;
    outCol = c;
}
