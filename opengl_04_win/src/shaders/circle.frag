#version 430

in vec4 v_color;
in vec2 v_texcoord;

uniform vec4 u_color;

out vec4 color;

void main()
{
    float softness = 3;
    vec2 uv = v_texcoord;
    uv = uv * 2.0 - 1.0;
    vec2 dp = fwidth(uv);
    softness *= dp.x;
    float d = smoothstep(1 - softness * 2, 1 - softness, length(uv)) - smoothstep(1 - softness, 1.0, length(uv));
    color = vec4(u_color.xyz, d * u_color.a);
}
