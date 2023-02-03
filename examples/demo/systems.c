#include "systems.h"

#include <jot/jot.h>

void system_player_input(GameData* data) {
  Player* player = &data->player;

  if (jot_key_down(JOT_KEY_W)) {
    player->transform.velocity.y = 8.0f;
  }
  if (jot_key(JOT_KEY_A)) {
    player->transform.velocity.x -= 0.35f;
  }
  if (jot_key(JOT_KEY_D)) {
    player->transform.velocity.x += 0.35f;
  }
}

// apply physics to single transform
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
  if (transform->position.y - transform->size.y / 2 < -PIXEL_HEIGHT / 2.0f) {
    transform->position.y = -PIXEL_HEIGHT / 2.0f + transform->size.y / 2;
    transform->velocity.y = 0;
  }

  // wall collision
  if (transform->position.x - transform->size.x / 2 > PIXEL_WIDTH / 2.0f || transform->position.x + transform->size.x / 2 < -PIXEL_WIDTH / 2.0f) {
    transform->position.x *= -0.999;
  }
}

void system_physics(GameData* data) {
  do_physics(&data->player.transform);
}

