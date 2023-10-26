#version 430

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;

uniform mat4 mvp;

out vec4 v_color;

void main()
{
    gl_Position = mvp * vec4(a_pos, 1.0);
    v_color = vec4(a_color, 1.0);
}
// #version 110
// uniform mat4 MVP;
// attribute vec3 vCol;
// attribute vec2 vPos;
// varying vec3 color;
// void main()
// {
//     gl_Position = MVP * vec4(vPos, 0.0, 1.0);
//     color = vCol;
// }