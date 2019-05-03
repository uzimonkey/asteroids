#include "ecs.h"
#include "vid.h"
#include <stdio.h>
#include <SDL_events.h>

void print_it(EcsID id) {
  POS *pos = ecs_get_component(id, POS);
  printf("%f,%f\n", pos->x, pos->y);
}

int main(void) {
  vid_start();
  ecs_start();

  for(;;) {
    for(SDL_Event e; SDL_PollEvent(&e);) {
      if(e.type == SDL_QUIT) goto done;
    }

    vid_begin_frame();
    vid_draw_rect((Rect){0, 0, 10, 10}, (Color){255, 255, 255, 255});
    vid_end_frame();
  }

done:
  ecs_stop();
  vid_stop();
}
