#version 330 core

in vec2 TexCoord;
in vec3 VertexColor;

out vec4 FragColor;

uniform sampler2D tex;
uniform float mixValue;

void main()
{
    // Use vertex color instead of position-based color
    vec3 colorMode = VertexColor;

    // Get texture color
    vec4 textureColor = texture(tex, TexCoord);

    // Mix between color mode and texture mode
    vec3 finalColor = mix(colorMode, textureColor.rgb, mixValue);

    FragColor = vec4(finalColor, 1.0);
}