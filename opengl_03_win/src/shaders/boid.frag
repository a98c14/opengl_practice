#version 430

uniform vec4 u_color;

in vec4 v_color;
in vec2 v_texcoord;

out vec4 color;

float sd_triangle_isosceles(in vec2 p, in vec2 q)
{
    p.x = abs(p.x);
    vec2 a = p - q*clamp( dot(p,q)/dot(q,q), 0.0, 1.0 );
    vec2 b = p - q*vec2( clamp( p.x/q.x, 0.0, 1.0 ), 1.0 );
    float s = -sign( q.y );
    vec2 d = min(vec2(dot(a,a), s*(p.x*q.y-p.y*q.x) ),
                 vec2(dot(b,b), s*(p.y-q.y)));
    return -sqrt(d.x)*sign(d.y);
}

void main()
{
    vec2 uv = v_texcoord;
    uv = uv * 2.0 - 1.0;
    uv = uv + vec2(0.0, 0.8);
    float d = sd_triangle_isosceles(uv, vec2(0.8, 1.6));
    d = smoothstep(0.0, 0.2, d);
    d = 1 - d;
    color = vec4(d, d, d, d);
}
