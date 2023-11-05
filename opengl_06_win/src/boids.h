#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "opus.h"
#include "opus.c"
#include "fonts.h"

#define BUCKET_BOID_CAPACITY 256

typedef struct 
{
    int32 x, y;

    uint16 boid_count;
    uint16* boid_indices;
    Vec2* boid_positions;
    Vec2* boid_directions;
} BoidBucket;

typedef struct
{
    float32 cell_size;

    int32* bucket_keys;
    uint16 bucket_count;
    BoidBucket* buckets;

    Arena* arena;
} BoidBucketHashMap;

typedef struct
{
    uint8 bucket_count;
    uint16* buckets;
} BoidBucketLookupArray;

internal BoidBucketHashMap*
boid_bucket_new(Arena* arena, float32 width, float32 height, float32 visual_range);

internal BoidBucket*
get_bucket(BoidBucketHashMap* hash_map, Vec2 pos);

internal void
insert_to_bucket(BoidBucketHashMap* hash_map, Vec2 pos, uint16 boid_index);

internal void
calculate_bucket_indices(Arena* scratch_arena, BoidBucketHashMap* hash_map, Vec2* in_boid_positions, uint16* out_bucket_indices, BoidBucketLookupArray* out_lookup_buckets, uint16 boid_count);