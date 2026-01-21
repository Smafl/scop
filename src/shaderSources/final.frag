#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D tex;
uniform float mixValue;

void main() {
    vec4 colorFromTexture = texture(tex, TexCoord);
    vec4 flatColor = vec4(0.8, 0.8, 0.8, 1.0);  // Light gray
    FragColor = mix(flatColor, colorFromTexture, mixValue);
}