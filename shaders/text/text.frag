#version 410 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float dist = texture(text, TexCoords).r;

    float smoothing = clamp(fwidth(dist), 0.0, 0.1);

    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);
    color = vec4(textColor, alpha);
}
