#include "mathlib.h"

#define RAD_2_DEG (180.0 / PI)
#define DEG_2_RAD (PI / 180.0)

double lerp(double a, double b, double t) {
  return (1.0-t)*a + t*b;
}

double sign(double a) {
  return a >= 0 ? 1.0 : -1.0;
}

Vec2 rotate(Vec2 v, double a) {
  a *= DEG_2_RAD;
  return (Vec2){
    .x = v.x*cos(a) - v.y*sin(a),
    .y = v.y*cos(a) + v.x*sin(a)
  };
}
