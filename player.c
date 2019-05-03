#include "ecs.h"
#include "vid.h"
#include "event.h"

static EcsID pid = {.id=-1};
static POS *pos;

static bool find_player(void) {
  pid = ecs_find(ecs_component_mask(POS), ecs_flag_mask(PLAYER));
  if(pid.id == -1) {
    pos = NULL;
    return false;
  }

  pos = ecs_get_component(pid, POS);
  if(pos == NULL)
    return false;

  return true;
}


void player_sys_move(void) {
  if(!ecs_is_valid(pid))
    find_player();

  pos->x += event_key_down(RIGHT) - event_key_down(LEFT);
}

void player_spawn(void) {
  EcsID pid = ecs_create_entity();
  *(POS*)ecs_add_component(pid, POS) = (POS){SCENE_WIDTH/2,SCENE_HEIGHT-10};
  *(SIZE*)ecs_add_component(pid, SIZE) = (SIZE){10, 10};
  ecs_set_flag(pid, RECT);
  ecs_set_flag(pid, PLAYER);
}
