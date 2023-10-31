#version 430

in vec4 v_color;
in vec2 v_texcoord;

uniform vec4 u_color;

out vec4 color;

float sd_box(in vec2 p, in vec2 b)
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main()
{
    float softness = 2;
    vec2 uv = v_texcoord;
    uv = uv * 2.0 - 1.0;
    uv = abs(uv);
    vec2 p = fwidth(uv);
    float a_x = 1 - smoothstep(1.0 - softness * p.x, 1.0, uv.x);
    float a_y = 1 - smoothstep(1.0 - softness * p.y, 1.0, uv.y);
    float a = min(a_x, a_y) * max(a_x, a_y);
    color = vec4(u_color.xyz, a * u_color.a);
}
