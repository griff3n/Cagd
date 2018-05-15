#version 330

out vec4 fragColor;

uniform vec3 edgeColor;
uniform float thickness;
uniform vec4 viewport;
uniform vec3 background;

in Data
{
    vec3 color;

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
    float d  = distance_line_point(data.v0, data.v1, gl_FragCoord.xy);
    float mixValue = smoothstep(thickness - 1.0, thickness + 1.0, d);

    vec3 base  = data.color;
    vec4 edge  = vec4(edgeColor, 1.0);
    vec4 color = vec4(base, 1.0);

    fragColor = mix(color, edge, mixValue);
}
