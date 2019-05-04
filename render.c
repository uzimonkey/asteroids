#include "ecs.h"
#include "vid.h"

static void sprite(EcsID ent) {
  POS *p = ecs_get_component(ent, POS);
  TEX *t = ecs_get_component(ent, TEX);
  vid_draw_sprite(*t, p->x, p->y);
}

static void sprites(void) {
  ecs_iterate(ecs_component_mask(POS) | ecs_component_mask(TEX), 0, sprite);
}

void render_sys_render(void) {
  sprites();
}

