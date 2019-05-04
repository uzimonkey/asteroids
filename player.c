#include "ecs.h"
#include "vid.h"
#include "event.h"
#include "player.h"
#include "mathlib.h"

static bool initialized = false;
static EcsID pid = {.id=-1};
static TEX *tex;
static TRANSFORM *xform;
static PHYSICS *physics;

static void player_start(void) {
  pid = ecs_create_entity();
  ecs_set_immortal(pid, true);
  ecs_set_flag(pid, PLAYER);
  ecs_set_flag(pid, WRAP);

  xform = ecs_add_component(pid, TRANSFORM);

  tex = ecs_add_component(pid, TEX);
  *tex = vid_load_texture("assets/" SPRITE);

  physics = ecs_add_component(pid, PHYSICS);

  ecs_kill_entity(pid);
  initialized = true;
}

void player_sys(float delta_time) {
  // Torque
  double torque_input = event_key_down(RIGHT) - event_key_down(LEFT);
  if(torque_input) {
    physics->angular_drag = 0;
    physics->angular_vel += torque_input * TORQUE * delta_time;
  } else {
    physics->angular_drag = lerp(
        physics->angular_drag,
        ANGULAR_DRAG,
        ANGULAR_DRAG_SENSITIVITY * delta_time);
  }

  if(fabs(physics->angular_vel) > MAX_ANGULAR_VEL)
    physics->angular_vel = MAX_ANGULAR_VEL * sign(physics->angular_vel);

  // Thrust
  if(event_key_down(UP)) {
    physics->drag = 0;
    Vec2 thrust_input = rotate((Vec2){0, -THRUST}, xform->rot);
    physics->vel = (Vec2){
      .x = physics->vel.x + thrust_input.x * delta_time,
      .y = physics->vel.y + thrust_input.y * delta_time};
  } else {
    physics->drag = lerp(physics->drag, DRAG, DRAG_SENSITIVITY * delta_time);
  }
}

void player_spawn(void) {
  if(!initialized)
    player_start();

  ecs_kill_entity(pid);
  xform->pos = (Vec2){SCENE_WIDTH/2,SCENE_HEIGHT/2};
  xform->rot = 0;
  *physics = (PHYSICS){
    .vel = (Vec2){0,0},
    .angular_vel = 0,
    .drag = DRAG,
    .angular_drag = ANGULAR_DRAG};
  ecs_reincarnate(pid);
}
