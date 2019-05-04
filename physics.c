#include "ecs.h"
#include <math.h>

bool physics_start(void) {
  return true;
}

void physics_stop(void) {
}

const char *physics_get_error(void) {
  return "";
}

static double dt;
static void physics(EcsID ent) {
  TRANSFORM *xform = ecs_get_component(ent, TRANSFORM);
  PHYSICS *phys = ecs_get_component(ent, PHYSICS);

  double drag = (phys->angular_vel * phys->angular_vel) * phys->angular_drag;
  phys->angular_vel += drag * dt * (phys->angular_vel > 0 ? -1 : 1);
  //double drag = fabs(phys->angular_vel) * phys->angular_drag;
  //phys->angular_vel *= 1.0 - dt * drag;

  xform->pos.x += phys->vel.x * dt;
  xform->pos.y += phys->vel.y * dt;
  xform->rot   += phys->angular_vel * dt;
}

void physics_sys(double delta_time) {
  dt = delta_time;

  ecs_iterate(
      ecs_component_mask(TRANSFORM) | ecs_component_mask(PHYSICS),
      0, physics);
}
