#version 330

out vec4 fragColor;

uniform float thickness;
uniform vec4 viewport;
uniform vec3 background;

uniform usampler2D idTexture;

in Data
{
    vec3 color;

    flat uint id0;
    flat uint id1;

    vec2 v0;
    vec2 v1;
} data;

float distance_line_point(vec2 v, vec2 w, vec2 p)
{
    float l2 = (w.x - v.x) * (w.x - v.x) + (w.y - v.y) * (w.y - v.y);

    if (l2 == 0.0)
        return distance(p, v);

    float t = dot(p - v, w - v) / l2;

    if (t < 0.0)
        return distance(p, v);
    else if (t > 1.0)
        return distance(p, w);

    vec2 projection = v + t * (w - v);

    return distance(p, projection);
}

void main()
{
    uint id = texelFetch(idTexture, ivec2(gl_FragCoord.xy), 0).r;

    float d  = distance_line_point(data.v0, data.v1, gl_FragCoord.xy);
    float mixValue  = smoothstep(thickness - 1.0, thickness + 1.0, d);

    float alpha = 1.0;

    // If the fragments ids don't fit the current id make the edge transparent.
    if (id != 0U && id != data.id0 && id != data.id1)
        alpha = 0.0;

    vec3 base  = data.color;
    vec4 edge  = vec4(base, 0.0);
    vec4 color = vec4(base, alpha);

    fragColor = mix(color, edge, mixValue);
}
