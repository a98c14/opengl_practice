#include <corecrt_math.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

#include <opus.h>
#include <opus.c>

#include "editor.h"
#include "app.h"
#include "kinematics.h"

#include "editor.c"
#include "app.c"
#include "kinematics.c"

int main(void)
{
    Engine* e = engine_init();
    DrawContext* dc = e->draw_context;
    Theme* t = e->theme;

    /* app initialization */
    float32 dt_scale = 1;

    float32 world_width = e->renderer->world_width;
    float32 world_height = e->renderer->world_height;
    float32 world_half_width = world_width / 2.0f;
    float32 world_half_height = world_height / 2.0f;

    const int32 joint_count = 3;
    const float32 length = 100;
    const float32 thickness = 3;
    const Vec4 color = color_to_vec4(ColorWhite);
    float32 angles[3] = { 30, 60, 90};

    /* main loop */
    while (!glfwWindowShouldClose(e->window->glfw_window))
    {
        engine_frame_start(e);
        float32 dt = e->time.dt * dt_scale;

        // draw_line(dc, vec2(-20, -1), vec2(20, -1), ColorWhite, 3.5);
        // draw_line(dc, vec2(0, 0), vec2(-100, 100), ColorWhite, 3);
        // draw_line(dc, vec2(-100, 100), vec2(0, 100), ColorWhite, 3);
        // draw_line(dc, vec2(0, 100), vec2(0, 200), ColorWhite, 3);
        // draw_circle(dc, vec2(-100, 100), 20, ColorWhite);
        // draw_circle(dc, vec2(0, 100), 20, ColorWhite);
        // draw_circle(dc, vec2(0, 200), 20, ColorWhite);
        // draw_line_fixed(dc, vec2(10, 10 * sin(e->time.current_frame / 1000)), 100, 0, ColorWhite, 3);
        // draw_line_fixed(dc, vec2(10, 10 * sin(e->time.current_frame / 1000)), 100, 360 * (fmod(e->time.current_frame / 1000.0f, 1.0f)), ColorRed500, 3);
        // draw_line_fixed(dc, vec2(10, 10 * sin(e->time.current_frame / 1000)), 100, 45, ColorBlue500, 3);
        
        Mat4 scale = mat4_scale(vec3(length, thickness, 0));
        DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_line, joint_count);
        Vec2 root_position = vec2_zero();
        Vec2 current_position = root_position;

        angles[0] = 360 * fmod(e->time.current_frame / 4000.0f, 1.0f);
        angles[1] = 360 * sin(e->time.current_frame / 2000.0f);
        angles[2] = 360 * cos(e->time.current_frame / 1000.0f);
        
        for(int32 i = 0; i < joint_count; i++)
        {
            Vec2 position = current_position;
            float32 angle = angles[i];
            float32 radian = angle * PI_FLOAT32 / 180.0;
            float32 cosx = (float32)cosf(radian);
            float32 sinx = (float32)sinf(radian);
            draw_circle(dc, position, 20, ColorWhite);
            draw_arrow(dc, position, 20, angle+90, ColorBlue500, 2);
            draw_arrow(dc, position, 20, angle, ColorRed500, 2);
            position.x += cosx * (length / 2.0f);
            position.y += sinx * (length / 2.0f);
            current_position.x += cosx * length;
            current_position.y += sinx * length;
            Mat4 translation = mat4_translation(vec3_xy_z(position, 0));
            Mat4 rotation = mat4_rotation(angle);
            draw_buffer.model_buffer[i] = mat4_transform(translation, rotation, scale);
            ((ShaderDataLine*)draw_buffer.uniform_data_buffer)[i].color = color;
        }
        draw_circle(dc, current_position, 20, ColorRed500);

        /** TODO:
         * - draw a control panel
         * - set angle of each joint separately
         * - draw arrows from joins
         */
        

        /** Control Panel */

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}