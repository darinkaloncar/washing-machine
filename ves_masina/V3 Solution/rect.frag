#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform bool hasHat;
uniform sampler2D uTex0;
uniform sampler2D uTex1; 

void main()
{
    if(!hasHat) outCol = texture(uTex0, chTex);
    else {
        vec4 hatCol = texture(uTex1, chTex);
        if (hatCol.a == 0.0f) {
            outCol = texture(uTex0, chTex);
        } else {
            outCol = hatCol;
        }
    }
} 