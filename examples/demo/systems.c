#include "systems.h"
#include "types.h"

#include <jot/jot.h>
#include <stdio.h>


// system_player_input
// -------------------
void system_player_input(GameData* data) {
  Player* player = &data->player;

  if (jot_key_down(JOT_KEY_W)) {
    player->transform.velocity.y = 12.0f;
  }
  if (jot_key(JOT_KEY_A)) {
    player->transform.velocity.x -= 0.35f;
  }
  if (jot_key(JOT_KEY_D)) {
    player->transform.velocity.x += 0.35f;
  }
}


// system_physics
// --------------
static void do_physics(Transform* transform) {
  // gravity
  transform->velocity.y -= 0.4f;

  // air resistance
  transform->velocity.x *= 0.9f;
  transform->velocity.y *= 0.99f;

  // move
  transform->position.x += transform->velocity.x;
  transform->position.y += transform->velocity.y;

  // floor collision
  if (transform->position.y - transform->size.y / 2 < 0.0f) {
    transform->position.y = transform->size.y / 2;
    transform->velocity.y = 0;
  }

  // wall collision
  if (transform->position.x - transform->size.x / 2 > PIXEL_WIDTH || transform->position.x + transform->size.x / 2 < 0.0f) {
    transform->position.x *= -0.999;
  }
}

void system_physics(GameData* data) {
  do_physics(&data->player.transform);
}


// system_collision
// ----------------
static void handle_level_collision(Level* level, Transform* transform) {
  int bottom_x = (int)((transform->position.x) / 16.0f + 0.5f);
  int bottom_y = (int)((transform->position.y - transform->size.y) / 16.0f + 0.5f);

  collision_type collider = level->collision_map[bottom_y * LEVEL_COLUMNS + bottom_x];

  if (collider == COLLISION_TDLR) {
    printf("HELLO\n");
    float diff = ((float)bottom_y * 16.0f + 24.0f) - transform->position.y;
    if (diff > 0) {
      transform->position.y += diff;
      transform->velocity.y = 0;
    }
  }
}

void system_collision(GameData* data) {
  handle_level_collision(&data->level, &data->player.transform);  
}
