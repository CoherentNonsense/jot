#include "jot.h"
#include <math.h>
#include <stdio.h>

#define PIXEL_WIDTH 300
#define PIXEL_HEIGHT 200
#define MAX_ENEMIES 20

typedef struct Vec2 {
  float x;
  float y;
} Vec2;

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

typedef struct GameData {
  Player player;
  Enemy enemies[MAX_ENEMIES];
} GameData;

void move_player(Player* player) {
  if (jot_key_down(JOT_KEY_W)) {
    player->transform.velocity.y = 10.0f;
  }
  if (jot_key(JOT_KEY_A)) {
    player->transform.velocity.x -= 0.5f;
  }
  if (jot_key(JOT_KEY_S)) {
    player->transform.velocity.y -= 1.0f;
  }
  if (jot_key(JOT_KEY_D)) {
    player->transform.velocity.x += 0.5f;
  }
}

void apply_physics(Transform* transform) {
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
    transform->position.x *= -1;
  }
}

GameData data;
float anim_timer;
int anim_frame;
int main() {
  jot_init("Demo Game", PIXEL_WIDTH, PIXEL_HEIGHT);

  jot_texture("resources/spritesheet.png");

  data.player.transform.size.y = 16;
  data.player.transform.size.x = 9;

  while (jot_update()) {
    float time = jot_get_time();
    Vec2* player_pos = &data.player.transform.position;

    move_player(&data.player);
    apply_physics(&data.player.transform);

    Player* player = &data.player;
    player->tongue[0] = player->transform.position;
    player->tongue[0].y += 1.0f;
    player->tongue[0].x += 4.0f;
    for (int i = 1; i < 5; ++i) {
      player->tongue[i].y -= 1.0f;
      if ((player->tongue[i].x - player->tongue[i - 1].x) * (player->tongue[i].x - player->tongue[i - 1].x) > 900) {
        player->tongue[i] = player->tongue[i - 1];
      } else {
        player->tongue[i].x += (player->tongue[i-1].x - player->tongue[i].x) * 0.4f;
        player->tongue[i].y += (player->tongue[i-1].y - player->tongue[i].y) * 0.4f;
      }
    }

    jot_clear(0.0f, 0.0f, 0.0f);

    if (anim_timer < time) {
      anim_timer = time + 0.1f;
      anim_frame = (anim_frame + 1) % 2;
    }
    jot_draw_sprite(16 * anim_frame, 0, 16, 16, player_pos->x, player_pos->y, 16, 16 + player->transform.velocity.y, player->transform.velocity.x / 15.0f);
    float arm_angle = atan2f(jot_cursor_y() - player_pos->y, jot_cursor_x() - player_pos->x);
    jot_draw_line(player_pos->x, player_pos->y, 3, player_pos->x + 10.0f * cosf(arm_angle), player_pos->y + 10.0f * sinf(arm_angle), 4, 1.0f, 1.0f, 0.0f);

    for (int i = 1; i < 5; ++i) {
      Vec2* pos = &data.player.tongue[i];
      Vec2* prev_pos = &data.player.tongue[i - 1];
      jot_draw_line(prev_pos->x, prev_pos->y, 1, pos->x, pos->y, 1, 1.0f, 0.1f, 0.2f);
    }
  }  

  return 0;
}