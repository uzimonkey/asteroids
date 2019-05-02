#include "ecs.h"
#include <stdio.h>

void print_it(EcsID id) {
  POS *pos = ecs_get_component(id, POS);
  printf("%f,%f\n", pos->x, pos->y);
}

int main(void) {
  ecs_init();
  for(int i = 0; i < 10; i++) {
    EcsID id = ecs_create_entity();
    *(POS*)ecs_add_component(id, POS) = (POS){.x=i, .y=i};
  }

  ecs_iterate_component(POS, print_it);
  printf("%d\n", sum(1,2,3));
}
