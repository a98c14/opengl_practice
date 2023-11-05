#include "boids.h"

internal BoidBucketHashMap*
boid_bucket_new(Arena* arena, float32 width, float32 height, float32 visual_range)
{
    uint32 bucket_count_x = (uint32)(3 * (width / visual_range));
    uint32 bucket_count_y = (uint32)(3 * (height / visual_range));
    BoidBucketHashMap* hash_map = arena_push_struct_zero(arena, BoidBucketHashMap);
    hash_map->bucket_count = bucket_count_x * bucket_count_y;
    hash_map->cell_size = visual_range;
    hash_map->buckets = arena_push_array_zero(arena, BoidBucket, hash_map->bucket_count);
    hash_map->bucket_keys = arena_push_array_zero(arena, int32, hash_map->bucket_count);
    hash_map->arena = arena;
    memset(hash_map->bucket_keys, -1, sizeof(int32) * hash_map->bucket_count);
    for(int i = 0; i < hash_map->bucket_count; i++)
    {
        hash_map->buckets[i].boid_indices = arena_push_array(arena, uint16, BUCKET_BOID_CAPACITY);
        hash_map->buckets[i].boid_directions = arena_push_array(arena, Vec2, BUCKET_BOID_CAPACITY);
        hash_map->buckets[i].boid_positions = arena_push_array(arena, Vec2, BUCKET_BOID_CAPACITY);
    }
    return hash_map;
}

internal void
calculate_bucket_indices(Arena* scratch_arena, BoidBucketHashMap* hash_map, Vec2* in_boid_positions, uint16* out_bucket_indices, BoidBucketLookupArray* out_lookup_buckets, uint16 boid_count)
{
    // find position keys
    for(int i = 0 ; i < boid_count; i++)
    {
        Vec2 pos = in_boid_positions[i];

        // actual bucket
        int16 bucket_x = (int16)(pos.x / hash_map->cell_size) + (pos.x > 0 ? 0 : -1);
        int16 bucket_y = (int16)(pos.y / hash_map->cell_size) + (pos.y > 0 ? 0 : -1);

        // lookup bucket range
        int16 min_bucket_x = (int16)((pos.x - hash_map->cell_size) / hash_map->cell_size) + (pos.x > 0 ? 0 : -1);
        int16 max_bucket_x = (int16)((pos.x + hash_map->cell_size) / hash_map->cell_size) + (pos.x > 0 ? 0 : -1);
        int16 min_bucket_y = (int16)((pos.y - hash_map->cell_size) / hash_map->cell_size) + (pos.y > 0 ? 0 : -1);
        int16 max_bucket_y = (int16)((pos.y + hash_map->cell_size) / hash_map->cell_size) + (pos.y > 0 ? 0 : -1);

        BoidBucketLookupArray* lookup_bucket = &out_lookup_buckets[i];
        int8 lookup_bucket_count = (1 + max_bucket_x - min_bucket_x) * (1 + max_bucket_y - min_bucket_y);
        lookup_bucket->bucket_count = 0;
        lookup_bucket->buckets = arena_push_array(scratch_arena, uint16, lookup_bucket_count);
        for(int16 y = min_bucket_y; y <= max_bucket_y; y++)
        {
            for(int16 x = min_bucket_x; x <= max_bucket_x; x++)
            {
                int32 key = ((int32)x << 16) + ((int32)y);
                int32 index = hash_uint64(key) % hash_map->bucket_count;
                int32 initial_index = index;
                int32 existing_key = hash_map->bucket_keys[index];
                while(existing_key != key && existing_key != -1)
                {
                    index++;
                    existing_key = hash_map->bucket_keys[index];
                    Assert(index - initial_index < 10, "probe count exceeded maximum");
                }

                // if this is the first time this bucket is encountered, initialize it
                if(existing_key == -1) 
                {                    
                    hash_map->buckets[index].x = x;
                    hash_map->buckets[index].y = y;
                    hash_map->buckets[index].boid_count = 0;
                    hash_map->bucket_keys[index] = key;
                }

                // if this is the actual bucket set the bucket index
                if(x == bucket_x && y == bucket_y) out_bucket_indices[i] = index;

                // set the lookup index
                lookup_bucket->buckets[lookup_bucket->bucket_count] = index;
                lookup_bucket->bucket_count++;
            }
        }
    }
}