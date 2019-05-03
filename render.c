#include "ecs.h"
#include "vid.h"

static void render_rectangle(EcsID eid) {
  POS *p = ecs_get_component(eid, POS);
  SIZE *s = ecs_get_component(eid, SIZE);
  vid_draw_rect(p->x, p->y, s->y, s->x);
}

static void render_rectangles(void) {
  static EcsMask cmask = ecs_component_mask(POS) | ecs_component_mask(SIZE);
  static EcsMask fmask = ecs_flag_mask(RECT);
  ecs_iterate(cmask, fmask, render_rectangle);
}

void sys_render(void) {
  render_rectangles();
}

