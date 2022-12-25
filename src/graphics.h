#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cglm/types.h>

void graphics_init(const char* title, const vec2 size);
void graphics_terminate();

int graphics_should_close();
struct GLFWwindow* graphics_get_window();
float graphics_get_screen_scale();
void graphics_poll();

unsigned int graphics_load_texture(const char* path);
void graphics_use_texture(unsigned int texture);

void graphics_start_draw();
void graphics_draw(const vec2 position, const vec2 uv, const vec2 size, const float rotation);
// void graphics_draw_color(vec2 position, vec2 uv, vec2 size, float rotation, vec3 color);
void graphics_draw_circle(const vec2 position, const float radius, const vec3 color);
void graphics_draw_rect(const vec2 position, const vec2 size, const float rotation, const vec3 color);
void graphics_clear(const float r, const float g, const float b);
void graphics_end_draw();

#endif