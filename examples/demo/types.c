#include "types.h"
#include <stdlib.h>

static void player_init(Player* player) {
  player->transform.size.x = 9;
  player->transform.size.y = 16;

  Animator* animator = &player->animator;
  animator->next_frame = 0.0f;
  animator->length = 1;
  animator->current = 0;
  animator->spritesheets = malloc(sizeof(Spritesheet) * animator->length);

  // run anim
  animator->spritesheets[0].length = 1;
  animator->spritesheets[0].uv = (Vec2){0, 0};
  animator->spritesheets[0].speed = 0.1f;
}

static void player_exit(Player* player) {
  
}

void game_data_init(GameData* data) {
  player_init(&data->player);

  // temp level
  for (int x = 0; x < LEVEL_COLUMNS; ++x) {
    for (int y = 0; y < LEVEL_ROWS; ++y) {
      if (x == 0 || y == 0 || x == LEVEL_COLUMNS - 1 || y == LEVEL_ROWS - 1) {
        data->level.tiles[y * LEVEL_COLUMNS + x] = TILE_BLOCK;
      } else {
        data->level.tiles[y * LEVEL_COLUMNS + x] = TILE_AIR;
      }

      data->level.collision_map[y * LEVEL_COLUMNS + x] = COLLISION_NONE;
      if (y == 0) {
        data->level.collision_map[y * LEVEL_COLUMNS + x] |= COLLISION_TOP;
      }
    }
  }
  
  data->level.tiles[5 * LEVEL_COLUMNS + 7] = TILE_BLOCK;
  data->level.collision_map[5 * LEVEL_COLUMNS + 7] = COLLISION_TOP;
}

void game_data_exit(GameData* data) {
  player_exit(&data->player);
}