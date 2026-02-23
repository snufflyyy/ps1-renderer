#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D text;

void main() {
    vec4 texture_color = texture(text, TexCoords);
    if (texture_color.a < 0.1) {
        discard;
    }
    FragColor = texture_color;
}
