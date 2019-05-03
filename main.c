#include "ecs.h"
#include "vid.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL_events.h>

void die(const char *message) {
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

#define start(subsystem) if(!subsystem##_start()) die(subsystem##_get_error());
#define stop(subsystem) subsystem##_stop();



void move_player(void) {
  static EcsID pid;
  static POS *pos;

  if(!ecs_is_valid(pid)) {
    pid = ecs_find(ecs_component_mask(POS), ecs_flag_mask(PLAYER));
    if(pid.id == -1)
      return;
    pos = ecs_get_component(pid, POS);
  }

  pos->x += event_key_down(RIGHT) - event_key_down(LEFT);
}


void render_rectangle(EcsID eid) {
  POS *p = ecs_get_component(eid, POS);
  SIZE *s = ecs_get_component(eid, SIZE);
  vid_draw_rect(p->x, p->y, s->y, s->x);
}

void render_rectangles(void) {
  static EcsMask cmask = ecs_component_mask(POS) | ecs_component_mask(SIZE);
  static EcsMask fmask = ecs_flag_mask(RECT);
  ecs_iterate(cmask, fmask, render_rectangle);
}

void render(void) {
  vid_begin_frame();
  render_rectangles();
  vid_end_frame();
}

int main(void) {
  start(event);
  start(vid);
  start(ecs);

  {
    EcsID pid = ecs_create_entity();
    *(POS*)ecs_add_component(pid, POS) = (POS){SCENE_WIDTH/2,SCENE_HEIGHT-10};
    *(SIZE*)ecs_add_component(pid, SIZE) = (SIZE){10, 10};
    ecs_set_flag(pid, RECT);
    ecs_set_flag(pid, PLAYER);
  }

  for(;;) {
    if(event_process() || event_key_pressed(ESCAPE))
      goto done;

    move_player();
    render();
  }

done:
  stop(ecs);
  stop(vid);
  stop(event);
}
