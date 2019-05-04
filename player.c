#include "ecs.h"
#include "vid.h"
#include "event.h"

static bool initialized = false;
static EcsID pid = {.id=-1};
static TEX *tex;
static POS *pos;

static void player_start(void) {
  pid = ecs_create_entity();
  ecs_set_immortal(pid, true);
  ecs_set_flag(pid, PLAYER);

  pos = ecs_add_component(pid, POS);
  *pos = (POS){SCENE_WIDTH/2,SCENE_HEIGHT/2};

  tex = ecs_add_component(pid, TEX);
  *tex = vid_load_texture("assets/ship.png");

  ecs_kill_entity(pid);
  initialized = true;
}

void player_sys_move(void) {
  pos->x += event_key_down(RIGHT) - event_key_down(LEFT);
}

void player_spawn(void) {
  if(!initialized)
    player_start();

  ecs_kill_entity(pid);
  pos->x = SCENE_WIDTH/2;
  pos->y = SCENE_HEIGHT/2;
  ecs_reincarnate(pid);
}
