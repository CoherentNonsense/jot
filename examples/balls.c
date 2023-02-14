#include "utils/vec2.h"

#include <jot/jot.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BALL_COUNT 2000
#define WIDTH 1600
#define HEIGHT 900

typedef struct Ball {
  Vec2 position;
  Vec2 last_position;
  Vec2 acceleration;
  float radius;
  float hue;
} Ball;

// game data
// ---------
int spawned = 0;
float spawn_timer = 0.0f;
float game_time = 0.0f;
const float radius = 400.0f;
struct Ball balls[BALL_COUNT];

void spawn_ball() {
  if (spawned < BALL_COUNT) {
    balls[spawned].last_position = (Vec2){ 801.0f, 701.0f };
    balls[spawned].position = (Vec2){ 800.0f, 700.0f };
    balls[spawned].radius = 8.0f;
    spawned += 1;
  }
}

int main() {
  open_window("Balls", WIDTH, HEIGHT);
      
  // game loop
  // ---------
  game_time = get_time();
  while (update_window()) {
    // get timing
    float new_time = get_time();
    float delta_time = new_time - game_time;
    game_time = new_time;
    
    
    if (1.0f / delta_time > 60.0f && game_time > spawn_timer) {
      spawn_timer = game_time + 0.025f;
      spawn_ball();
    }
        
    // update balls
    const int steps = 8;
    const float sub_delta_time = delta_time / (float)steps;
    for (int step = 0; step < steps; ++step) {
      
      for (int i = 0; i < spawned; ++i) {
        Ball* ball = &balls[i];

        ball->acceleration.y -= 1000.0f;
        
        {
          const Vec2 offset = vec2_sub(ball->position, (Vec2){ 800.0f, 450.0f });
          const float mag = vec2_mag(offset);
          if (mag > radius - ball->radius) {
            ball->position = vec2_add((Vec2){ 800.0f, 450.0f }, vec2_mul(vec2_normalise(offset), radius - ball->radius));
          }
        }
      
        for (int j = i + 1; j < spawned; ++j) {
          Ball* other = &balls[j];
          const Vec2 offset = vec2_sub(other->position, ball->position);
          const float mag = vec2_mag(offset);
          const float collision_dist = ball->radius + other->radius;
          if (mag < collision_dist) {
            other->position = vec2_add(other->position, vec2_mul(vec2_normalise(offset), (collision_dist - mag) * 0.5f));
            ball->position = vec2_sub(ball->position, vec2_mul(vec2_normalise(offset), (collision_dist - mag) * 0.5f));
          }
        }
        
        const Vec2 velocity = vec2_sub(ball->position, ball->last_position);
        ball->last_position = ball->position;
        // position = position + velocity + acceleration * dt * dt
        ball->position = vec2_add(ball->position, vec2_add(velocity, vec2_mul(ball->acceleration, sub_delta_time * sub_delta_time)));
        ball->acceleration = (Vec2){ 0.0f, 0.0f };
      }
    }
     
    clear_screen(0.05f, 0.06f, 0.08f);
    fill_circle(800.0f, 450.0f, radius, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < spawned; ++i) {
      Ball* ball = &balls[i];
      fill_circle(ball->position.x, ball->position.y, ball->radius, 0.2f, 1.0f, 0.5f);
    }
  }
  
  close_window();
  
  return 0;
}