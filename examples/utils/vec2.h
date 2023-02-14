#ifndef VEC2_H
#define VEC2_H

#include "math.h"

typedef struct Vec2 {
  float x;
  float y;
} Vec2;

float lerp(float a, float b, float n) {
  return a + (b - a) * n;
}

Vec2 vec2_lerp(Vec2 a, Vec2 b, float n) {
  return (Vec2) {
    lerp(a.x, b.x, n),
    lerp(a.y, b.y, n)
  };
}

Vec2 vec2_add(const Vec2 a, const Vec2 b) {
  return (Vec2) {
    a.x + b.x,
    a.y + b.y
  };
};

Vec2 vec2_sub(const Vec2 a, const Vec2 b) {
  return (Vec2){
    a.x - b.x,
    a.y - b.y
  };
}

Vec2 vec2_subs(const Vec2 vec, const float scalar) {
  return (Vec2){
    vec.x - scalar,
    vec.y - scalar
  };
}

Vec2 vec2_mul(const Vec2 vec, const float scalar) {
  return (Vec2){
    vec.x * scalar,
    vec.y * scalar
  };
}

float vec2_mag(const Vec2 vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

float vec2_mag2(const Vec2 vec) {
  return vec.x * vec.x + vec.y * vec.y;
}

Vec2 vec2_normalise(const Vec2 vec) {
  float magnitude = vec2_mag(vec);
  return (Vec2){
    vec.x / magnitude,
    vec.y / magnitude
  };
}

#endif