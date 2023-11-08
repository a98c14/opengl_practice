#include <opus.h>
#include <opus.c>
#include "app.h"
#include "app.c"

internal void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main(void)
{
    /* initialization */
    logger_init();


    Arena* persistent_arena = make_arena_reserve(mb(512));
    Arena* frame_arena = make_arena_reserve(mb(256));
    Window* window = window_create(persistent_arena, WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Example", key_callback);
    RendererConfiguration renderer_configuration = {
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .world_height = 27,
        .clear_color = ColorSlate900
    };

    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);
    DrawContext* dc = draw_context_new(persistent_arena, renderer);
    EngineTime time = engine_time_new();

    int32 world_width = renderer->camera.world_width;
    int32 world_height = renderer->camera.world_height;
    int32 cell_count = world_height * world_width;
    TextureIndex world_texture = texture_new(renderer, world_width, world_height, 4, GL_NEAREST, 0);

    Cell* cells = arena_push_array_zero_aligned(persistent_arena, Cell, cell_count, 16);
    Color* texture_data = arena_push_array_zero_aligned(persistent_arena, Color, cell_count, 16);

    /* application state */
    float32 simulation_clock = 0;
    float32 simulation_rate = 0.05;

    /* main loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        Profiler main_frame = profiler_begin(string("MainFrame"));
        /* frame: init */
        time = engine_get_time(time);
        InputMouse mouse = input_mouse_get(window, renderer->camera);

        /* frame: update */
        simulation_clock += time.dt;
        Profiler update = profiler_begin(string("Update"));
        int32 mouse_cell_x = (int32)(mouse.world.x + world_width / 2.0f);
        int32 mouse_cell_y = (int32)(mouse.world.y + world_height / 2.0f);
        bool32 mouse_is_in_bounds = mouse_cell_x >= 0 && mouse_cell_x < world_width && mouse_cell_y >= 0 && mouse_cell_y < world_height;

        if((mouse.button_state & MouseButtonStateLeft) > 0 && mouse_is_in_bounds)
        {
            cells[mouse_cell_y*world_width+mouse_cell_x].type = (uint8)CellMaterialTypeSand;
        }

        /* physics */
        if(simulation_clock > simulation_rate)
        {
            simulation_clock -= simulation_rate;
            for(int32 y = 0; y < world_height; y++)
            {
                for(int32 x = 0; x < world_width; x++)
                {
                    int32 i = y*world_width+x;
                    Cell* c = &cells[i];
                    if(c->type == CellMaterialTypeSand)
                    {
                        int32 ib = (y-1)*world_width+x;
                        int32 ilb = (y-1)*world_width+(x-1);
                        int32 irb = (y-1)*world_width+(x+1);
                        bool32 down_empty = y-1 >= 0 && cells[ib].type == CellMaterialTypeAir;
                        bool32 down_left_empty = y-1 >= 0 && x-1 >= 0 && cells[ilb].type == CellMaterialTypeAir;
                        bool32 down_right_empty = y-1 >= 0 && x+1 < world_width && cells[irb].type == CellMaterialTypeAir;
                        if(down_empty)
                        {
                            cells[i].type = CellMaterialTypeAir;
                            cells[i].priority = 0;
                            cells[ib].priority = 1;
                            cells[ib].type = CellMaterialTypeSand;
                        }
                        else if(down_right_empty)
                        {
                            cells[i].type = CellMaterialTypeAir;
                            cells[i].priority = 0;
                            cells[irb].priority = 1;
                            cells[irb].type = CellMaterialTypeSand;
                        }
                        else if(down_left_empty)
                        {
                            cells[i].type = CellMaterialTypeAir;
                            cells[i].priority = 0;
                            cells[ilb].priority = 1;
                            cells[ilb].type = CellMaterialTypeSand;
                        }
                        else
                        {
                            cells[i].priority = 3;
                            cells[i].type = CellMaterialTypeSand;
                        }
                    }
                }
            }
        }

        /* update colors */
        ShaderDataText shader_data = {0};
        shader_data.color = color_to_vec4(ColorBlack);
        shader_data.outline_color = color_to_vec4(ColorInvisible);
        shader_data.thickness = 0.50;
        shader_data.softness = 30;
        shader_data.outline_thickness = 0.2;
        
        for(int32 y = 0; y < world_height; y++)
        {
            for(int32 x = 0; x < world_width; x++)
            {                
                int32 cell_index = y*world_width+x;
                Cell* cell = &cells[cell_index];
                Vec4 current_cell_color = CellMaterialColors[cell->type];
                texture_data[cell_index] = vec4_to_color(current_cell_color);
            }
        }

        texture_update(renderer, world_texture, texture_data);
        draw_texture_aligned(dc, vec3_zero(), vec2(world_width, world_height), world_texture);
        profiler_end(&update);

        for(int32 y = 0; y < world_height; y++)
        {
            for(int32 x = 0; x < world_width; x++)
            {                
                int32 cell_index = y*world_width+x;
                Cell* cell = &cells[cell_index];
                draw_text(dc, vec2(x - world_width / 2.0f + 0.25f, y - world_height / 2.0f + 0.25), string_pushf(frame_arena, "%2d", cell->priority), ColorSlate700, 0.5f);
            }
        }

        /* frame: render */
        Profiler render = profiler_begin(string("Render"));
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(&render);
        profiler_end(&main_frame);
        arena_reset(frame_arena);


        float32 screen_left = -renderer->camera.world_width / 2;
        float32 screen_top = renderer->camera.world_height / 2;
        draw_text(dc, vec2(screen_left, screen_top), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), ColorBlack, 0.6f);
        draw_text(dc, vec2(screen_left, screen_top-1), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), ColorBlack, 0.6f);
        draw_text(dc, vec2(screen_left, screen_top-2), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), ColorBlack, 0.6f);
        draw_text(dc, vec2(screen_left, screen_top-3), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), ColorBlack, 0.6f);
        draw_text(dc, vec2(screen_left, screen_top-4), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), ColorBlack, 0.6f);
        draw_text(dc, vec2(screen_left, screen_top-5), string_pushf(frame_arena, "MouseCell: [%0.02d, %0.0d]", mouse_cell_x, mouse_cell_y), ColorBlack, 0.6f);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}