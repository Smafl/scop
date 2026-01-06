#version 410 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 VertexColor;

uniform sampler2D tex;
uniform float mixValue;

void main() {
    vec4 colorFromVertices = vec4(VertexColor, 1.0);
    vec4 colorFromTexture = texture(tex, TexCoord);

    FragColor = mix(colorFromVertices, colorFromTexture, mixValue);
}