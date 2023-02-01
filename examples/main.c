#include <jot/jot.h>
#include <math.h>

int main() {
  
  // initialisation
  // --------------
  // window
  jot_init("Demo", 320, 180);

  // texture
  jot_texture("resources/spritesheet.png");

  // game loop
  // ---------
  while (jot_update()) {
    float time = jot_get_time();

    jot_clear(0.1f, 0.2f, 0.3f);

    float ship_x = sinf(time) * 40.0f;
    float ship_y = cosf(time) * 40.0f;

    jot_draw_sprite(0, 0, 16, 16, ship_x, ship_y, 16, 16, -time);
  }

  // shutdown
  // --------
  jot_terminate();

  return 0;
}
