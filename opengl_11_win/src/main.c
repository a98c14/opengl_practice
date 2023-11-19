#include <corecrt_math.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

#include <opus.h>
#include <opus.c>

#include "editor.h"
#include "app.h"
#include "boids.h"

#include "editor.c"
#include "app.c"
#include "boids.c"

int main(void)
{
    Engine* e = engine_init();
    DrawContext* dc = e->draw_context;
    Theme* t = e->theme;

    /* app initialization */
    Rect window_rect = rect(200, 100, 180, 100);
    bool32 is_expanded = false;
    float32 dt_scale = 1;


    float32 avoid_range = 16;
    float32 visual_range = 32;
    float32 avoid_factor = 0.1;
    float32 alignment_factor = 0.01;
    float32 cohesion_factor = 0.01;
    float32 min_speed = 12;
    float32 max_speed = 64;
    float32 sight_range = -0.2;
    int32 boid_count = 800;
    Vec2* positions = arena_push_array_zero(e->persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(e->persistent_arena, Vec2, boid_count);
    float32* rotations = arena_push_array_zero(e->persistent_arena, float32, boid_count);
    

    srand(632939018);
    for(int32 i = 0; i < boid_count; i++)
    {
        float32 x = ((rand() % 10000) / 10000.0f) * 2 - 1;
        float32 y = ((rand() % 10000) / 10000.0f) * 2 - 1;
        positions[i] = mul_vec2_f32(vec2(x, y), 100);
        directions[i] = mul_vec2_f32(vec2_right(), 50);
    }

    float32 world_width = e->renderer->world_width;
    float32 world_height = e->renderer->world_height;
    float32 world_half_width = world_width / 2.0f;
    float32 world_half_height = world_height / 2.0f;

    /* main loop */
    while (!glfwWindowShouldClose(e->window->glfw_window))
    {
        engine_frame_start(e);
        float32 dt = e->time.dt * dt_scale;

        float32 visual_range_sqr = powf(visual_range, 2);
        float32 avoid_range_sqr = powf(avoid_range, 2);

        BoidBucketHashMap* hash_map = boid_bucket_hash_map_new(e->frame_arena, world_width, world_height, visual_range*2);
        uint16* boid_bucket_indices = arena_push_array(e->frame_arena, uint16, boid_count);
        BoidBucketLookupArray* boid_lookup_buckets = arena_push_array(e->frame_arena, BoidBucketLookupArray, boid_count);
        Vec2* cohesion_vectors = arena_push_array_zero(e->frame_arena, Vec2, boid_count);
        Vec2* alignment_vectors = arena_push_array_zero(e->frame_arena, Vec2, boid_count);
        Vec2* avoid_vectors = arena_push_array_zero(e->frame_arena, Vec2, boid_count);
        float32* speeds = arena_push_array_zero(e->frame_arena, float32, boid_count);


        /* preparation */
        calculate_bucket_indices(e->frame_arena, hash_map, positions, boid_bucket_indices, boid_lookup_buckets, boid_count);
        fill_buckets(hash_map, boid_bucket_indices, positions, directions, boid_count);

        /* rules */
        for(int i = 0; i < boid_count; i++)
        {
            Vec2 boid_pos = positions[i];
            Vec2 boid_dir_normalized = norm_vec2(directions[i]);
            Vec2 avoid_vector = vec2_zero();
            Vec2 group_direction = vec2_zero();
            Vec2 group_center = vec2_zero();
            uint16 group_count = 0;

            for(int j = 0; j < boid_lookup_buckets[i].count; j++)
            {
                uint16 bucket_index = boid_lookup_buckets[i].indices[j];
                BoidPositionBucket position_bucket = hash_map->position_buckets[bucket_index];
                BoidDirectionBucket direction_bucket = hash_map->direction_buckets[bucket_index];
                for(int p = 0; p < position_bucket.count; p++)
                {
                    Vec2 pos = position_bucket.positions[p];
                    Vec2 dir = direction_bucket.directions[p];

                    // check if boid can see the target and its not behind of the boid
                    if(dot_vec2(boid_dir_normalized, norm_vec2(sub_vec2(pos, boid_pos))) < sight_range) continue;
                    float32 d = distsqr_vec2(pos, boid_pos);

                    // if the distance is 0 (or almost 0) it means this boid is actually
                    // itself so we don't add that to the group. Can we find a better way to skip itself?
                    if(d < EPSILON_FLOAT32) continue;

                    if(d < avoid_range_sqr)
                    {
                        Vec2 boid_avoid_vector = mul_vec2_f32(sub_vec2(boid_pos, pos), avoid_factor);
                        avoid_vector = add_vec2(avoid_vector, boid_avoid_vector);
                    }

                    if(d < visual_range_sqr)
                    {
                        group_direction = add_vec2(group_direction, dir);
                        group_center = add_vec2(pos, group_center);
                        group_count++;

                        
                        if(i == 0)
                        {
                            draw_line(dc, pos, boid_pos, ColorTeal200, 1.8);
                        }
                    }
                }
            }

            if(group_count > 0)
            {
                group_center = div_vec2_f32(group_center, group_count);
                group_direction = div_vec2_f32(group_direction, group_count);
                cohesion_vectors[i] = mul_vec2_f32(sub_vec2(group_center, boid_pos), cohesion_factor);
                alignment_vectors[i] = mul_vec2_f32(group_direction, alignment_factor);
                avoid_vectors[i] = avoid_vector;
            }
        }

        for(int i = 0; i < boid_count; i++)
        {
            directions[i] = add_vec2(directions[i], mul_vec2_f32(cohesion_vectors[i], dt));
            directions[i] = add_vec2(directions[i], mul_vec2_f32(alignment_vectors[i], dt));
            directions[i] = add_vec2(directions[i], mul_vec2_f32(avoid_vectors[i], dt));
        }

        // calculate speeds
        for(int i = 0; i < boid_count; i++)
        {
            speeds[i] = len_vec2(directions[i]);
        }

        // speed limits
        for(int i = 0; i < boid_count; i++)
        {
            float32 speed = speeds[i];
            if(speed < min_speed)
            {
                float32 new_speed = lerp_f32(speed, min_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }

            if(speed > max_speed)
            {
                float32 new_speed = lerp_f32(speed, max_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }
        }

        /* move */
        for(int32 i = 0; i < boid_count; i++ )
        {
            Vec2 new_pos = add_vec2(positions[i], mul_vec2_f32(directions[i], dt));
            // teleport to the otherside of the screen if out of vision
            positions[i].x = fmod((new_pos.x + world_half_width + world_width), world_width) - world_half_width;
            positions[i].y = fmod((new_pos.y + world_half_height + world_height), world_height) - world_half_height;
        }

        /* calculate rotations */
        for(int32 i = 0; i < boid_count; i++ )
            rotations[i] = angle_vec2(directions[i]);

        /* draw */
        for(int32 i = 0; i < boid_count; i++)
            draw_boid(dc, positions[i], rotations[i], 2, ColorWhite);

        /* editor */
        for(int32 i = 0; i < 1; i++)
        {
            draw_line(dc, positions[i], add_vec2(positions[i], avoid_vectors[i]), ColorRed400, 2);
            draw_line(dc, positions[i], add_vec2(positions[i], cohesion_vectors[i]), ColorAmber200, 2);
            draw_line(dc, positions[i], add_vec2(positions[i], mul_vec2_f32(directions[i], 0.5)), ColorSlate300, 1.2);
            draw_circle(dc, positions[i], visual_range*2, ColorWhite);
            draw_circle(dc, positions[i], avoid_range*2, ColorRed400);
        }

        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("simulation"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            uint32 row_count = 6;
            LayoutGrid layout = layout_grid(rect_anchor(rect_from_wh(window.header.w, em(2) * row_count), window.header, ANCHOR_TL_TL), 3, row_count, e->theme->p2);
            ui_container(e->ctx, layout_grid_container(layout), t->container_light);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 0), string_pushf(e->frame_arena, "sim.speed: %0.2f", dt_scale), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 0, 2, 1), uuid_new(2, 0), range(0, 4), &dt_scale, t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 1), string_pushf(e->frame_arena, "avoidance: %0.2f", avoid_factor), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 1, 2, 1), uuid_new(3, 0), range(0, 1), &avoid_factor, t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 2), string_pushf(e->frame_arena, "cohesion: %0.2f", cohesion_factor), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 2, 2, 1), uuid_new(4, 0), range(0, 1), &cohesion_factor, t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 3), string_pushf(e->frame_arena, "alignment: %0.2f", alignment_factor), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 3, 2, 1), uuid_new(5, 0), range(0, 1), &alignment_factor, t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 4), string_pushf(e->frame_arena, "visual_range: %0.0f", visual_range), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 4, 2, 1), uuid_new(6, 0), range(8, 128), &visual_range, t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 5), string_pushf(e->frame_arena, "avoid_range: %0.0f", avoid_range), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 5, 2, 1), uuid_new(7, 0), range(8, 128), &avoid_range, t->slider_default);
        }

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}