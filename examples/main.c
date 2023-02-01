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

    jot_draw_sprite(0, 0, 16, 16, sin(time) * 40.0f, cos(time) * 40.0f, 16, 16, -time);
  }

  // shutdown
  // --------
  jot_terminate();

  return 0;
}
