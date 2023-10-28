#include "opus_graphics_primitives.h"

internal Geometry
geometry_quad_create()
{
    uint32 vao;
    uint32 vbo;
    uint32 ebo;

    // generate object names
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // bind buffers
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices_quad), shape_vertices_quad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape_indices_quad), shape_indices_quad, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float32), (void*)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float32), (void*)(3 * sizeof(float32)));
    glEnableVertexAttribArray(1);

    // texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float32), (void*)(6 * sizeof(float32)));
    glEnableVertexAttribArray(2);

    Geometry geom = {0};
    geom.index_count = 6;
    geom.vertex_array_object = vao;
    return geom;
}

internal Geometry
geometry_triangle_create()
{
    uint32 vao;
    uint32 ebo;
    uint32 vbo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices_triangle), shape_vertices_triangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape_indices_triangle), shape_indices_triangle, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32), (void*)(sizeof(float32) * 3));
    glEnableVertexAttribArray(1);

    Geometry geom = {0};
    geom.index_count = 3;
    geom.vertex_array_object = vao;
    return geom;
}

/* Transforms */
internal Mat4
transform_quad(Vec2 position, Vec2 scale, float32 rotation)
{
    Mat4 translation_mat = mat4_translation(vec3_xy_z(position, 0));
    Mat4 rotation_mat = mat4_rotation(rotation);
    Mat4 scale_mat = mat4_scale(vec3_xy_z(scale, 0));
    return mat4_transform(translation_mat, rotation_mat, scale_mat);
}

internal Mat4
transform_line(Vec2 start, Vec2 end, float32 thickness)
{
    Vec2 center = lerp_vec2(start, end, 0.5f);
    float32 dist = dist_vec2(end, start);
    float32 angle = angle_between_vec2(end, start);
    Mat4 translation = mat4_translation(vec3_xy_z(center, 0));
    Mat4 rotation = mat4_rotation(angle);
    Mat4 scale = mat4_scale(vec3(dist, thickness, 0));
    return mat4_transform(translation, rotation, scale);
}