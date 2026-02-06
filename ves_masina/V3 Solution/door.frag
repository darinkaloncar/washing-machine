#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTex;
uniform float uAlpha;

void main()
{
    vec4 t = texture(uTex, vUV);
    FragColor = vec4(t.rgb, t.a * uAlpha);
}
