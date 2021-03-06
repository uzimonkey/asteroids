#ifndef MATH_H
#define MATH_H
#include <math.h>

#define PI 3.14159265358979323846

typedef struct {
  float x, y;
} Vec2;

double lerp(double a, double b, double t);
double sign(double a);

Vec2 rotate(Vec2 v, double a);
double magnitude(Vec2 v);
double sqr_magnitude(Vec2 v);
Vec2 normalize(Vec2 v);
Vec2 invert(Vec2 v);

#endif
