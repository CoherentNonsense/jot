#include <jot/jot.h>
#include <stdio.h>
#include <math.h>

int main() {
  
  // initialisation
  // --------------
  // window
  jot_init("Demo", 320, 180);

  // texture
  jot_load_tex("resources/spritesheet.png");

  // game loop
  // ---------
  printf("STARTING LOOP\n");
  while (jot_update()) {
    float time = jot_get_time();
         
    jot_clear(0.1f, 0.2f, 0.3f);    
    
    jot_draw_sprite(0, 0, 16, 16, sin(time) * 20.0f, cos(time) * 20.0f, 16, 16, -time);
  }

  // shutdown
  // --------
  jot_terminate();

  return 0;
}
