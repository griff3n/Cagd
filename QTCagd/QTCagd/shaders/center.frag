#version 330
out vec4 fragColor;

uniform usampler2D idTexture;

in Data
{
    vec3 color;
    flat uint id;
} DataIn;

void main()
{
    uint id = texelFetch(idTexture, ivec2(gl_FragCoord.xy), 0).r;

    float alpha = 1.0;

    if (id != 0U && DataIn.id != id)
        alpha = 0.0;

    vec3 color = DataIn.color;

    fragColor = vec4(color, alpha);
}
