#pragma once
#include "app.h"
#define BUCKET_BOID_CAPACITY 256

typedef struct 
{
    uint16 count;
    Vec2* positions;
} BoidPositionBucket;

typedef struct 
{
    uint16 count;
    Vec2* directions;
} BoidDirectionBucket;

typedef struct
{
    float32 cell_size;
    uint16 bucket_count;
    BoidPositionBucket* position_buckets;
    BoidDirectionBucket* direction_buckets;
} BoidBucketHashMap;

typedef struct
{
    uint8 count;
    uint16* indices;
} BoidBucketLookupArray;

internal BoidBucketHashMap*
boid_bucket_hash_map_new(Arena* arena, float32 width, float32 height, float32 visual_range);

internal void
calculate_bucket_indices(Arena* scratch_arena, BoidBucketHashMap* hash_map, Vec2* in_boid_positions, uint16* out_bucket_indices, BoidBucketLookupArray* out_lookup_buckets, uint16 boid_count);

internal void
fill_buckets(BoidBucketHashMap* hash_map, uint16* boid_bucket_indices, Vec2* boid_positions, Vec2* boid_directions, uint16 boid_count);