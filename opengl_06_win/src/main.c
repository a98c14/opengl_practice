#include "boids.h"
#include "boids.c"

internal void
error_callback(int error, const char* description)
{
    fprintf(stderr, "[ERROR] %s\n", description);
}

internal void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

internal void
message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	char* source_str;
    switch (source)
    {
		case GL_DEBUG_SOURCE_API: source_str = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source_str = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER: source_str = "OTHER"; break;
        default: source_str = "NO_SOURCE"; break;
    }

	char* type_str;
    switch (type)
    {
		case GL_DEBUG_TYPE_ERROR: type_str = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_MARKER: type_str = "MARKER"; break;
		case GL_DEBUG_TYPE_OTHER: type_str = "OTHER"; break;
        default: type_str = "NO_TYPE"; break;
    }

	char* severity_str;
    switch (severity)
    {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return; // severity_str = "NOTIFICATION"; break;
		case GL_DEBUG_SEVERITY_LOW: severity_str = "LOW"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity_str = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_HIGH: severity_str = "HIGH"; break;
        default: severity_str = "NO_SEVERITY"; break;
    }

	printf("[GL_%s][%s|%s] %s\n", severity_str, source_str, type_str, message);
}

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main(void)
{
    Arena* persistent_arena = make_arena_reserve(mb(256));
    Arena* frame_arena = make_arena_reserve(mb(128));

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);

    int32 width, height;
    glfwGetFramebufferSize(window, &width, &height);

    RendererConfiguration renderer_configuration = {
        .window_width = width,
        .window_height = height,
        .world_height = 100,
        .clear_color = ColorWhite
    };
    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);

    DrawContext* dc = draw_context_new(persistent_arena, renderer);

    float32 frame_time = (float32)glfwGetTime();
    float32 last_frame_time, dt;
    Geometry geometry = geometry_quad_create();

    float32 padding = 10;
    float32 bounds_left = -renderer->camera.world_width/2+padding;
    float32 bounds_right = renderer->camera.world_width/2-padding;
    float32 bounds_bottom = -renderer->camera.world_height/2+padding;
    float32 bounds_top = renderer->camera.world_height/2-padding;

    float32 close_range = 1.6;
    float32 visual_range = 8;
    float32 avoid_factor = 20;
    float32 alignment_factor = 1.4;
    float32 cohesion_factor = 1.0;
    float32 min_speed = 2;
    float32 max_speed = 25;
    float32 sight_range = -0.2;

    uint32 boid_count = 100;
    Vec2* positions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* alignment_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* avoidance_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* cohesion_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    int32 scouts[] = { 0, 4, 13, 17, 46, 2, 15 };

    srand(time(NULL));
    for(int i = 0; i < boid_count; i++)
    {
        float32 x = ((rand() % 10000) / 10000.0f) * 2 - 1;
        float32 y = ((rand() % 10000) / 10000.0f) * 2 - 1;
        positions[i] = mul_vec2_f32(vec2(x, y), 30);
        directions[i] = mul_vec2_f32(vec2(x, y), 30);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        arena_reset(frame_arena);

        BoidBucketHashMap* hash_map = boid_bucket_new(frame_arena, renderer->camera.world_width, renderer->camera.world_height, visual_range);
        /* timers */
        last_frame_time = frame_time;
        frame_time = (float32)glfwGetTime();
        dt = frame_time - last_frame_time;
        // dt *= 0.5;
        
        /* input */
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Vec2 mouse_raw = vec2(xpos, ypos);
        Vec2 mouse_world = mouse_world_position(mouse_raw, renderer->camera);

        draw_bounds(dc, bounds_left, bounds_right, bounds_bottom, bounds_top, ColorRed400);


        uint16* boid_bucket_indices = arena_push_array(frame_arena, uint16, boid_count);
        BoidBucketLookupArray* boid_lookup_buckets = arena_push_array(frame_arena, BoidBucketLookupArray, boid_count);
        calculate_bucket_indices(frame_arena, hash_map, positions, boid_bucket_indices, boid_lookup_buckets, boid_count);

        // fill buckets 
        for(int i = 0; i < boid_count; i++)
        {
            uint16 bucket_index = boid_bucket_indices[i];
            BoidBucket* bucket = &hash_map->buckets[bucket_index];
            bucket->boid_indices[bucket->boid_count] = i;
            bucket->boid_positions[bucket->boid_count] = positions[i];
            bucket->boid_directions[bucket->boid_count] = directions[i];
            bucket->boid_count++;
        }

        for(int i = 0; i < boid_count; i++)
        {
            avoidance_vectors[i] = vec2_zero();
            cohesion_vectors[i] = vec2_zero();
            alignment_vectors[i] = vec2_zero();
            Vec2 group_center = vec2_zero();
            int32 neighbour_count = 0;

            if(i == 0)
            {
                draw_circle(dc, positions[i], visual_range*2, ColorBlack);
            }

            BoidBucketLookupArray* lookup_buckets = &boid_lookup_buckets[i];
            for(int j = 0; j < lookup_buckets->bucket_count; j++)
            {
                uint16 bucket_index = lookup_buckets->buckets[j];
                BoidBucket* bucket = &hash_map->buckets[bucket_index];
                for(int k = 0; k < bucket->boid_count; k++)
                {
                    int32 index = bucket->boid_indices[k];
                    if(i == index) continue;

                    

                    if(dot_vec2(norm_vec2(directions[i]), norm_vec2(sub_vec2(bucket->boid_positions[k], positions[i]))) < sight_range) continue;

                    float32 dist = dist_vec2(positions[i], bucket->boid_positions[k]);
                    if(dist < close_range)
                    {
                        Vec2 avoidance_vector = sub_vec2(positions[i], bucket->boid_positions[k]);
                        avoidance_vectors[i] = add_vec2(avoidance_vectors[i], avoidance_vector);

                        if(i == 0)
                        {
                            draw_line(dc, positions[i], bucket->boid_positions[k], ColorRed500);
                        }
                    } 
                    else if(dist < visual_range)
                    {
                        alignment_vectors[i] = add_vec2(alignment_vectors[i], bucket->boid_directions[k]);
                        group_center = add_vec2(group_center, bucket->boid_positions[k]);
                        neighbour_count++;

                        if(i == 0)
                        {
                            draw_line(dc, positions[i], bucket->boid_positions[k], ColorGreenPastel);
                        }
                    }

                    if(i == 0 && dist < visual_range)
                    {
                        draw_line(dc, bucket->boid_positions[k], add_vec2(bucket->boid_positions[k], div_vec2_f32(bucket->boid_directions[k], 2)), ColorShadow);
                    }
                }

                if(i == 0)
                {
                    float32 bucket_x = bucket->x*hash_map->cell_size;
                    float32 bucket_y = bucket->y*hash_map->cell_size;
                    draw_bounds(dc, bucket_x, bucket_x+hash_map->cell_size, bucket_y, bucket_y+hash_map->cell_size, ColorGreenPastel);
                }
            }

            if(neighbour_count > 0)
            {
                alignment_vectors[i] = div_vec2_f32(alignment_vectors[i], neighbour_count);
                cohesion_vectors[i] = sub_vec2(div_vec2_f32(group_center, neighbour_count), positions[i]);
            }
        }

        for(int i = 0; i < boid_count; i++)
        {
            Vec2 direction = directions[i];
            Vec2 avoidance = mul_vec2_f32(avoidance_vectors[i], avoid_factor * dt);
            Vec2 cohesion = mul_vec2_f32(cohesion_vectors[i], cohesion_factor * dt);
            Vec2 alignment = sub_vec2(alignment_vectors[i], direction);
            alignment = mul_vec2_f32(alignment, alignment_factor * dt);

            direction = add_vec2(direction, avoidance);
            direction = add_vec2(direction, cohesion);
            direction = add_vec2(direction, alignment);
            directions[i] = direction;
        }

        // directions
        for(int i = 0; i < boid_count; i++)
        {
            if(positions[i].x < bounds_left)
                directions[i].x += fabsf((positions[i].x - bounds_left)) * 10 * dt;
             
            if(positions[i].x > bounds_right)
                directions[i].x -= fabsf((positions[i].x - bounds_right)) * 10 * dt;

            if(positions[i].y < bounds_bottom)
                directions[i].y += fabsf((positions[i].y - bounds_bottom)) * 10 * dt;

            if(positions[i].y > bounds_top)
                directions[i].y -= fabsf((positions[i].y - bounds_top)) * 10 * dt;
        }

        // speed limits
        for(int i = 0; i < boid_count; i++)
        {
            float32 speed = len_vec2(directions[i]);
            if(speed < min_speed)
            {
                float32 new_speed = lerp_f32(speed, min_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }

            if(speed < max_speed)
            {
                float32 new_speed = lerp_f32(speed, max_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }
        }

        for(int i = 0; i < boid_count; i++)
        {
            positions[i] = add_vec2(positions[i], mul_vec2_f32(directions[i], dt));
        }

        // draw boids
        draw_boid(dc, positions[0], directions[0], 1.2f, ColorRed400);
        for(int i = 1; i < boid_count; i++)
        {
            draw_boid(dc, positions[i], directions[i], 1.2f, ColorBlack);
        }

        // String str = string("!\"#$%%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        // draw_text(dc, vec2(-100, 0), str);
        float32 end_time = (float32)glfwGetTime();
        String fps_str = string_pushf(frame_arena, "%.4f ms", (end_time - frame_time) * 1000);
        draw_text(dc, vec2(-renderer->camera.world_width / 2.0f + 1, renderer->camera.world_height / 2.0f - 3), fps_str);
        renderer_render(renderer, dt);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}