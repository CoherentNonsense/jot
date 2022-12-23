#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cglm/types.h>

void graphics_init(const char* title, const vec2 size);
void graphics_terminate();

int graphics_should_close();
struct GLFWwindow* graphics_get_window();
void graphics_poll();

unsigned int graphics_load_texture(const char* path);
void graphics_use_texture(unsigned int texture);

void graphics_start_draw();
void graphics_draw(vec2 position, vec2 uv, vec2 size, float rotation);
// void graphics_draw_color(vec2 position, vec2 uv, vec2 size, float rotation, vec3 color);
void graphics_draw_circle(vec2 position, float radius, vec3 color);
void graphics_draw_rect(vec2 position, vec2 size, vec3 color);
void graphics_clear(const float r, const float g, const float b);
void graphics_end_draw();

#endif