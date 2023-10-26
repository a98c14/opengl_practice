#version 430

in vec4 v_color;

out vec4 color;

void main()
{
    color = v_color;
}
// #version 110
// varying vec3 color;
// void main()
// {
//     gl_FragColor = vec4(color, 1.0);
// }