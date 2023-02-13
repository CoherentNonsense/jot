#include "types.h"
#include "systems.h"

#include <jot.h>
#include <math.h>
#include <stdio.h>


GameData data;

int main() {

  // initialise
  // ----------
  jot_init("Demo Game", PIXEL_WIDTH, PIXEL_HEIGHT);
  jot_texture("resources/spritesheet.png");

  game_data_init(&data);


  // game loop
  // ---------
  while (jot_update()) {
    // calculate timing
    float new_time = jot_get_time();
    data.delta_time = new_time - data.time;
    data.time = new_time;

    Vec2* player_pos = &data.player.transform.position;

    // run game systems
    system_player_input(&data);
    system_physics(&data);
    system_collision(&data);
    system_animator(&data);

    // TODO: move to systems
    Player* player = &data.player;
    player->tongue[0] = player->transform.position;
    player->tongue[0].y += 2.0f;
    player->tongue[0].x += 2.0f;
    for (int i = 1; i < 5; ++i) {
      player->tongue[i].y -= 0.5f;
      player->tongue[i].y -= 0.9f * (sinf(data.time * 20.0f + i) + 2.0f + sinf(data.time * 8.0f + i * 2.0f)) / 2.0f;
      player->tongue[i].x -= 0.2f * (sinf(data.time * 13.0f) + 2.0f + sinf(data.time * 7.0f)) / 2.0f;
      if ((player->tongue[i].x - player->tongue[i - 1].x) * (player->tongue[i].x - player->tongue[i - 1].x) > 900) {
        player->tongue[i] = player->tongue[i - 1];
      } else {
        player->tongue[i].x += (player->tongue[i-1].x - player->tongue[i].x) * 0.4f;
        player->tongue[i].y += (player->tongue[i-1].y - player->tongue[i].y) * 0.4f;
      }
    }

    jot_clear(0.3f, 0.4f, 0.3f);

    float stretch = 0;
    if (player->transform.velocity.y * 2.0f > -4) {
      stretch += player->transform.velocity.y * 2.0f;
    }

    Spritesheet* spritesheet = &player->animator.spritesheets[player->animator.current];
    jot_draw_sprite(
      spritesheet->uv.x + (16.0f * spritesheet->current), spritesheet->uv.y,
      16, 16,
      player_pos->x, player_pos->y,
      16, 16 + stretch,
      player->transform.velocity.x / 15.0f
    );

    for (int i = 1; i < 5; ++i) {
      Vec2* pos = &data.player.tongue[i];
      Vec2* prev_pos = &data.player.tongue[i - 1];
      jot_draw_line(prev_pos->x, prev_pos->y, 1, pos->x, pos->y, 1, 1.0f, 0.1f, 0.2f);
    }

    float arm_angle = atan2f(jot_cursor_y() - player_pos->y, jot_cursor_x() - player_pos->x);
    jot_draw_line(player_pos->x, player_pos->y, 3, player_pos->x + 10.0f * cosf(arm_angle), player_pos->y + 10.0f * sinf(arm_angle), 4, 1.0f, 1.0f, 0.0f);

    // draw level
    for (int x = 0; x < LEVEL_COLUMNS; ++x) {
      for (int y = 0; y < LEVEL_ROWS; ++y) {
        if (data.level.tiles[y * LEVEL_COLUMNS + x] == TILE_BLOCK) {
          float pos_x = (float)x * 16.0f;
          float pos_y = (float)y * 16.0f;
          jot_fill_rect(pos_x + 8, pos_y + 8, 8, 8, 0, 1.0f, 0.8f, 0.5f);
        }
      }
    }

    jot_draw_sprite(0, 16, 16, 16, 24, 24, 16, 16, 0);
  }  


  // clean up
  game_data_exit(&data);
  jot_terminate();

  return 0;
}
