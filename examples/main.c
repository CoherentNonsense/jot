#include <jot/jot.h>
#include <cglm/vec2.h>

int main() {
  
  // initialisation
  // --------------
  // window
  jot_init("Demo", 400, 300);

  // texture
  jot_load_tex("resources/spritesheet.png");
  
  // game loop
  // ---------
  while (jot_update()) {
    jot_clear(JOT_BLUE);
    
    jot_draw_sprite(0, 0, 16, 16, 0, 0, 16, 16, 0.0f);
  }

  // shutdown
  // --------
  jot_terminate();

  return 0;
}
