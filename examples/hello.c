#include "jot.h"
#include <math.h>

int main() {
  
  // initialisation
  // --------------
  open_window("Demo", 320, 180);
  load_texture("resources/spritesheet.png");
  
  // game loop
  // ---------
  while (update_window()) {    
    float time = get_time();

    clear_screen(0.1f, 0.2f, 0.3f);

    float ship_x = sinf(time) * 40.0f + 160;
    float ship_y = cosf(time) * 40.0f + 90;

    draw_sprite(0, 0, 16, 16, ship_x, ship_y, 16, 16, -time);
  }

  // shutdown
  // --------
  close_window();

  return 0;
}
