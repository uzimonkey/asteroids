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

int main(void) {
  start(vid);
  start(event);
  start(ecs);

  for(;;) {
    if(event_process() || event_key_pressed(ESCAPE))
      goto done;

    vid_begin_frame();
    vid_draw_rect((Rect){0, 0, 10, 10});
    vid_end_frame();
  }

done:
  stop(ecs);
  stop(event);
  stop(vid);
}
