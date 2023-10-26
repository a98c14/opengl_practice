// #version 430

// layout(location = 0) in vec3 a_pos;
// layout(location = 1) in vec3 a_color;
// layout(location = 2) in vec2 a_tex_coord;

// uniform mat4 mvp;
// uniform vec4 color;

// out vec4 v_color;

// void main()
// {
//     gl_Position = mvp * vec4(a_pos, 1.0);
//     v_color = color;
// }
#version 110
uniform mat4 MVP;
attribute vec3 vCol;
attribute vec2 vPos;
varying vec3 color;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
}