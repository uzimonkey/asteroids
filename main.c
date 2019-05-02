#include "ecs.h"
#include <stdio.h>

int main(void) {
  ecs_init();
  EcsID id = ecs_create_entity();
  *(POS*)ecs_add_component(id, POS) = (POS){.x=10, .y=20};

  POS *pos = ecs_get_component(id, POS);
  printf("%f,%f\n", pos->x, pos->y);
}
