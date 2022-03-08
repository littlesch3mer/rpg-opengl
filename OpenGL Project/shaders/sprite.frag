#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{    
    vec4 texColor=texture2D(sprite, TexCoords);
    if(texColor.a < 0.01)
        discard;
    color = texColor;
}