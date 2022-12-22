#include <jot/jot.h>

#include <stdlib.h>
#include <time.h>

#define BALL_COUNT 100
#define WIDTH 320
#define HEIGHT 180

#define MAX_VELOCITY_X 40
#define MAX_VELOCITY_Y 50
#define GRAVITY 10

struct Vec2 {
  float x;
  float y;
};

struct Ball {
  struct Vec2 position;
  struct Vec2 velocity;
};

int main() {
  // initialisation
  // --------------
  // window
  jot_init("Demo", WIDTH, HEIGHT);
  
  // texture
  jot_load_tex("resources/spritesheet.png");
  
  // data
  // ----
  float game_time = 0.0f;
  struct Ball balls[BALL_COUNT];
  
  // initialise balls
  srand(time(0));
  for (int i = 0; i < BALL_COUNT; ++i) {
    balls[i].position.x = (i % 15) * 20.0f - (WIDTH / 2.0f) + 16.0f;
    balls[i].position.y = 0.0f;
    balls[i].velocity.x = (((float)rand() / RAND_MAX) - 0.5f) * MAX_VELOCITY_X;
    balls[i].velocity.y = ((float)rand() / RAND_MAX) * MAX_VELOCITY_Y;
  }
  
  // game loop
  // ---------
  while (jot_update()) {
    // get delta time
    float new_time = jot_get_time();
    float delta_time = new_time - game_time;
    game_time = new_time;
    
    // update balls
    for (int i = 0; i < BALL_COUNT; ++i) {
      struct Ball* ball = &balls[i];
      ball->velocity.y -= GRAVITY * delta_time;
      
      ball->position.x += ball->velocity.x * delta_time;
      ball->position.y += ball->velocity.y * delta_time;
      
      if (ball->position.y - 8.0f < -(HEIGHT / 2.0f) || ball->position.y + 8.0f > (HEIGHT / 2.0f)) {
        ball->velocity.y *= -1.0f;
      }
      
      if (ball->position.x - 8.0f < -(WIDTH / 2.0f) || ball->position.x + 8.0f > (WIDTH / 2.0f)) {
        ball->velocity.x *= -1.0f;
      }
    }
    
    // draw balls
    jot_clear(0.1f, 0.1f, 0.3f);
    for (int i = 0; i < BALL_COUNT; ++i) {
      jot_draw_sprite(0, 0, 16, 16, balls[i].position.x, balls[i].position.y, 16, 16, 0.0f);
    }
  }
  
  return 0;
}