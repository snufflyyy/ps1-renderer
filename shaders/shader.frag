#version 410 core
out vec4 fragment_color;

in vec2 in_texture_coords;

uniform sampler2D in_texture;

uniform float far;
uniform float near;

uniform vec3 fog_color;
uniform float fog_density;

float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = linearize_depth(gl_FragCoord.z);

    float fog_factor = exp(-fog_density * depth);

    vec4 texture_color = texture(in_texture, in_texture_coords);
    if (texture_color.a <= 0.0) {
        discard;
    }

    fragment_color = mix(vec4(fog_color, 1.0), texture_color, fog_factor);
}
