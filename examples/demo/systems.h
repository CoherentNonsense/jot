#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "types.h"

void system_player_input(GameData* data);
void system_physics(GameData* data);
void system_collision(GameData* data);

#endif