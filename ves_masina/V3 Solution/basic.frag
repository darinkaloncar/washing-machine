#version 330 core
out vec4 FragColor;

in vec2 vUV;

uniform vec4 uColor;       // used for all shapes
uniform float uAlpha;      // added to make transparent

void main()
{
    FragColor = vec4(uColor.rgb, uColor.a * uAlpha);
}
