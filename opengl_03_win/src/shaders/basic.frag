#version 430

in vec4 v_color;

uniform vec4 u_color;

out vec4 color;

void main()
{
    color = vec4(mix(v_color.xyz, u_color.xyz, u_color.a), u_color.a);
}
