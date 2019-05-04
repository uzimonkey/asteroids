#include "ecs.h"
#include "vid.h"
#include "event.h"
#include "render.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void die(const char *message) {
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

#define start(subsystem) if(!subsystem##_start()) die(subsystem##_get_error());
#define stop(subsystem) subsystem##_stop();

int main(void) {
  start(event);
  start(vid);
  start(ecs);

  player_spawn();
  for(;;) {
    if(event_process() || event_key_pressed(ESCAPE))
      goto done;

    player_sys_move();

    vid_begin_frame();
    render_sys_render();
    vid_end_frame();
  }

done:
  stop(ecs);
  stop(vid);
  stop(event);
}
