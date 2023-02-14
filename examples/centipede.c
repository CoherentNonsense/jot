#include "jot/jot.h"
#include "utils/vec2.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define SPEED 80
#define LIMB_DURATION 0.15
#define LIMBS 10 * 2
#define BODY_SEGS_PER_LIMB 3
#define BODY_SEGS ((LIMBS / 2) * BODY_SEGS_PER_LIMB - BODY_SEGS_PER_LIMB + 3)

typedef struct BodySegment {
  Vec2 pos;
  float angle;
} BodySegment;

typedef struct Limb {
  Vec2 foot;
  Vec2 target;
  Vec2 from;
  float start_time;
  uint8_t moving;
} Limb;

typedef struct Centipede {
  Vec2 head;
  BodySegment body_segments[BODY_SEGS];
  Limb limbs[LIMBS];
} Centipede;

// data
// ----
Centipede centipede;
float time;
// colors
float r1 = 0.6;
float g1 = 0.1f;
float b1 = 0.2f;
float r2 = 0.8f;
float g2 = 0.5f;
float b2 = 0.4f;

// solves the IK and draws a 2 segment leg
void draw_leg(Vec2 hip, Vec2 foot, int flip) {
  float limb_length_0 = 14;
  float limb_length_1 = 14;
  float hip_to_foot_dist = vec2_mag(vec2_sub(foot, hip));
  float cos_angle_0 = ((hip_to_foot_dist * hip_to_foot_dist) + (limb_length_0 * limb_length_0) - (limb_length_1 * limb_length_1)) / (2 * hip_to_foot_dist * limb_length_0);
  float angle_0 = acosf(cos_angle_0);

  float cos_angle_1 = ((limb_length_1 * limb_length_1) + (limb_length_0 * limb_length_0) - (hip_to_foot_dist * hip_to_foot_dist)) / (2 * limb_length_1 * limb_length_0);
  float angle_1 = acosf(cos_angle_1);

  if (flip) {
    angle_0 *= -1;
    angle_1 *= -1;
  }

  Vec2 hip_to_foot = vec2_sub(foot, hip);
  float atan = atan2f(hip_to_foot.y, hip_to_foot.x) - 3.14159/2;

  Vec2 knee = (Vec2){ hip.x - sinf(atan - angle_0) * limb_length_0, hip.y + cosf(atan - angle_0) * limb_length_0 };
  Vec2 result = (Vec2){ knee.x - sinf(atan - angle_0 + 3.14159 - angle_1) * limb_length_1, knee.y + cosf(atan - angle_0 + 3.14159 - angle_1) * limb_length_1 };
  draw_line(hip.x, hip.y, 5, knee.x, knee.y, 5, r1, g1, b1);
  draw_line(knee.x, knee.y, 5, result.x, result.y, 5, r1, g1, b1);
  fill_circle(knee.x, knee.y, 2.5f, r2, g2, b2);
  fill_circle(result.x, result.y, 2.5f, r2, g2, b2);
}

int main() {
  open_window("Centipede", 600, 400);

  while (update_window()) {

    // timing
    float new_time = get_time();
    float delta_time = new_time - time;
    time = new_time;


    // update gecko
    Vec2 cursor = (Vec2){
      cursor_x(),
      cursor_y()
    };

    // move head
    BodySegment* head = &centipede.body_segments[0];
    if (vec2_mag(vec2_sub(cursor, head->pos)) > 10) {
      Vec2 velocity = vec2_mul(vec2_normalise(vec2_sub(cursor, head->pos)), SPEED * delta_time);
      head->pos = vec2_add(head->pos, velocity);
    }

    // the rest of the body trails behind
    for (int i = 1; i < BODY_SEGS; ++i) {
      Vec2 dir = vec2_sub(centipede.body_segments[i].pos, centipede.body_segments[i - 1].pos);
      if (dir.x != 0 && dir.y != 0) {
        Vec2 offset = vec2_sub(vec2_add(centipede.body_segments[i - 1].pos, vec2_mul(vec2_normalise(dir), 10)), centipede.body_segments[i].pos);
        centipede.body_segments[i].pos = vec2_add(centipede.body_segments[i].pos, vec2_mul(offset, 0.25));
      }
    }

    // move the legs
    for (int i = 0; i < LIMBS; i += 2) {
      int body_index = (i / 2) * BODY_SEGS_PER_LIMB + 2;
      BodySegment* body = &centipede.body_segments[body_index];
      Limb* limb_l = &centipede.limbs[i];
      Limb* limb_r = &centipede.limbs[i + 1];

      // find the target destination of each leg (where the next step will land)
      Vec2 dir = vec2_sub(centipede.body_segments[body_index - 1].pos, centipede.body_segments[body_index].pos);
      float angle = atan2f(dir.y, dir.x);
      Vec2 target_l = (Vec2){ (body - 2)->pos.x - sinf(angle) * 15, (body - 2)->pos.y + cosf(angle) * 15 };
      Vec2 target_r = (Vec2){ (body - 2)->pos.x + sinf(angle) * 15, (body - 2)->pos.y - cosf(angle) * 15 };

      // 2 states | currently moving => interpolate to target
      //          | not moving       => if it's stretched too far, set it's target to the next position
      if (limb_l->moving) {
        float progress = (time - limb_l->start_time) / (float)LIMB_DURATION;
        limb_l->target = target_l;
        if (progress > 1) {
          limb_l->foot = limb_l->target;
          limb_l->moving = 0;
        } else {
          limb_l->foot = vec2_lerp(limb_l->from, limb_l->target, progress);
        }

      } else {
        if (!limb_r->moving && vec2_mag(vec2_sub(limb_l->foot, target_l)) > 25) {
          limb_l->start_time = time;
          limb_l->from = limb_l->foot;
          limb_l->target = target_l;
          limb_l->moving = 1;
        }
      }

      if (limb_r->moving) {
        float progress = (time - limb_r->start_time) / (float)LIMB_DURATION;
        limb_r->target = target_r;
        if (progress > 1) {
          limb_r->foot = limb_r->target;
          limb_r->moving = 0;
        } else {
          limb_r->foot = vec2_lerp(limb_r->from, limb_r->target, progress);
        }

      } else {
        if (!limb_l->moving && vec2_mag(vec2_sub(limb_r->foot, target_r)) > 25) {
          limb_r->start_time = time;
          limb_r->from = limb_r->foot;
          limb_r->target = target_r;
          limb_r->moving = 1;
        }
      }
    }

    // draw centipede
    clear_screen(0.09, 0.08, 0.09);
    BodySegment* body = centipede.body_segments;

    // draw the legs
    for (int i = 0; i < LIMBS; i += 2) {
      int body_index = (i / 2) * BODY_SEGS_PER_LIMB + 2;
      BodySegment* body = &centipede.body_segments[body_index];
      Limb* limb_l = &centipede.limbs[i];
      Limb* limb_r = &centipede.limbs[i + 1];

      Vec2 dir = vec2_sub(centipede.body_segments[body_index - 1].pos, centipede.body_segments[body_index].pos);
      float angle = atan2f(dir.y, dir.x);
      Vec2 hip_l = (Vec2){ body->pos.x - sinf(angle) * 10, body->pos.y + cosf(angle) * 10 };
      Vec2 hip_r = (Vec2){ body->pos.x + sinf(angle) * 10, body->pos.y - cosf(angle) * 10 };

      draw_leg(hip_l, limb_l->foot, 1);
      draw_leg(hip_r, limb_r->foot, 0);
    }

    // draw the body segments
    for (int i = BODY_SEGS - 1; i >= 0; --i) {
      float progress = (float)(BODY_SEGS - i) / BODY_SEGS;
      fill_circle(body[i].pos.x, body[i].pos.y, sinf((float)(i - 2) * 3.14159 * 2 / (float)BODY_SEGS_PER_LIMB + (3.14159 / 2)) * 1 + 10, lerp(r2, r1, progress), lerp(g2, g1, progress), lerp(b2, b1, progress));
    }

    // draw the head
    float head_angle1 = atan2f(cursor.y - head->pos.y, cursor.x - head->pos.x) + (3.14159f / 3.0f);
    float head_angle2 = atan2f(cursor.y - head->pos.y, cursor.x - head->pos.x) + (2 * 3.14159f / 3.0f);
    Vec2 antena_offset1 = (Vec2){ sinf(head_angle1), -cosf(head_angle1) };
    Vec2 antena_offset2 = (Vec2){ sinf(head_angle2), -cosf(head_angle2) };
    draw_line(head->pos.x + antena_offset1.x * 8, head->pos.y + antena_offset1.y * 8, 5, head->pos.x + antena_offset1.x * 20, head->pos.y + antena_offset1.y * 20, 3, r1, g1, b1);
    draw_line(head->pos.x + antena_offset2.x * 8, head->pos.y + antena_offset2.y * 8, 5, head->pos.x + antena_offset2.x * 20, head->pos.y + antena_offset2.y * 20, 3, r1, g1, b1);
  }

  close_window();

  return 0;
}