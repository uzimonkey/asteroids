#include "ecs.h"
#include "vid.h"

static void sprite(EcsID ent) {
  TRANSFORM *xform = ecs_get_component(ent, TRANSFORM);
  TEX *t = ecs_get_component(ent, TEX);
  vid_draw_sprite(*t, xform->pos.x, xform->pos.y, xform->rot);
}

static void sprites(void) {
  ecs_iterate(
      ecs_component_mask(TRANSFORM) | ecs_component_mask(TEX),
      0, sprite);
}

void render_sys_render(void) {
  sprites();
}

