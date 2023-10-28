#version 430

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 mvp;
uniform vec4 u_color;

out vec4 v_color;
out vec2 v_texcoord;

void main()
{
    gl_Position = mvp * vec4(a_pos, 1.0);
    v_color = vec4(a_color, 1.0);
    v_texcoord = a_tex_coord;
}
