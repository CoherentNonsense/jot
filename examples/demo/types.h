#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>


#define MAX_ENEMIES 20
#define MAX_BULLETS 20
#define MAX_PARTICLES 20


// basic components
// ----------------
typedef struct Vec2 {
  float x;
  float y;
} Vec2;

// something that interacts with the level (e.g. player, enemy)
typedef struct Transform {
  Vec2 position;
  Vec2 velocity;
  Vec2 size;
} Transform;

typedef struct Player {
  Transform transform;
  Vec2 tongue[5];
} Player;

typedef struct Enemy {
  Transform transform;
} Enemy;

typedef struct Bullet {
  Transform transform;
} Bullet;

typedef struct Particle {
  Transform transform;
} Particle;


// level
// -----
#define LEVEL_COLUMNS 30
#define LEVEL_ROWS 20

typedef uint8_t tile_type;

#define TILE_AIR 0
#define TILE_BLOCK 1

typedef uint8_t collision_type;

#define COLLISION_NONE 0
#define COLLISION_TDLR 1

typedef struct Level {
  tile_type tiles[LEVEL_COLUMNS * LEVEL_ROWS];
  collision_type collision_map[LEVEL_COLUMNS * LEVEL_ROWS];
} Level;


// game state
// ----------

// resolution of the game
#define PIXEL_WIDTH (LEVEL_COLUMNS * 16)
#define PIXEL_HEIGHT (LEVEL_ROWS * 16)

typedef struct GameData {
  float time;
  float delta_time;
  Player player;
  Enemy enemies[MAX_ENEMIES];
  Bullet bullets[MAX_BULLETS];
  Particle particles[MAX_PARTICLES];
  Level level;
} GameData;

#endif