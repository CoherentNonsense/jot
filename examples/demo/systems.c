#include "systems.h"
#include "types.h"

#include <jot/jot.h>
#include <stdio.h>


// system_player_input
// -------------------
void system_player_input(GameData* data) {
  Player* player = &data->player;

  if (get_key_down(KEY_W)) {
    player->transform.velocity.y = 10.0f;
  }
  if (get_key(KEY_A)) {
    player->transform.velocity.x -= 0.5f;
  }
  if (get_key(KEY_D)) {
    player->transform.velocity.x += 0.5f;
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
    transform->position.x = (PIXEL_WIDTH - transform->position.x) * 0.999f;
  }
}

void system_physics(GameData* data) {
  do_physics(&data->player.transform);
}


// system_collision
// ----------------
static void handle_level_collision(Level* level, Transform* transform) {
  // HACK: this whole thing is temporary
  int bottom_x = (int)((transform->position.x) / 16.0f + 0.5f);
  int bottom_y = (int)((transform->position.y - transform->size.y) / 16.0f + 0.5f);

  collision_type collider = level->collision_map[bottom_y * LEVEL_COLUMNS + bottom_x];
  if (collider & COLLISION_TOP) {
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


// system_animator
// ---------------
static void update_animator(Animator* animator, float time) {
  Spritesheet* spritesheet = &animator->spritesheets[animator->current];
  if (animator->next_frame < time) {
    animator->next_frame = time + spritesheet->speed;
    spritesheet->current = (spritesheet->current + 1) % spritesheet->length;
  }
}

void system_animator(GameData* data) {
  update_animator(&data->player.animator, data->time);
}