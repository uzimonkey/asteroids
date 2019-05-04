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

  // Exponential angular drag
  {
    double drag = (phys->angular_vel * phys->angular_vel) * phys->angular_drag;
    phys->angular_vel += drag * dt * (phys->angular_vel > 0 ? -1 : 1);
  }

  // Exponential drag
  phys->vel = (Vec2){
    .x = phys->vel.x +
      phys->vel.x*phys->vel.x * -sign(phys->vel.x) *phys->drag * dt,
    .y = phys->vel.y +
      phys->vel.y*phys->vel.y * -sign(phys->vel.y) *phys->drag * dt
  };

  // Movement and rotation
  xform->pos.x += phys->vel.x * dt;
  xform->pos.y += phys->vel.y * dt;
  xform->rot   += phys->angular_vel * dt;

  // Wrapping
  if(ecs_has_flag(ent, WRAP)) {
    while(xform->pos.x < 0)
      xform->pos.x += SCENE_WIDTH;
    while(xform->pos.x >= SCENE_WIDTH)
      xform->pos.x -= SCENE_WIDTH;

    while(xform->pos.y < 0)
      xform->pos.y += SCENE_HEIGHT;
    while(xform->pos.y >= SCENE_HEIGHT)
      xform->pos.y -= SCENE_HEIGHT;
  }
}

void physics_sys(double delta_time) {
  dt = delta_time;

  ecs_iterate(
      ecs_component_mask(TRANSFORM) | ecs_component_mask(PHYSICS),
      0, physics);
}
