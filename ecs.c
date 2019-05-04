#include "ecs.h"
#include <string.h>

// Unwrap functions
#undef ecs_add_component
#undef ecs_remove_component
#undef ecs_has_component
#undef ecs_get_component

static int num_entities;
static struct {
  int version;
  bool alive;
  bool immortal;
  EcsMask component_mask;
  EcsMask flag_mask;
  void *components[NUM_COMPONENTS];
} *entities;

static const char* error = "";


// Resize ecs to be able to hold new_size number of entities
static void resize(int new_size) {
  entities = realloc(entities, sizeof(*entities) * new_size);
  memset(&entities[num_entities], 0, sizeof(*entities)*(new_size-num_entities));
  num_entities = new_size;
}


//
// Utility Functions
//
// Is this EcsID valid?
bool ecs_is_valid(EcsID ent) {
  return ent.id >= 0 && ent.id < num_entities
    && entities[ent.id].version == ent.version;
}


// Get the last error
const char *ecs_get_error(void) {
  return error;
}


//
// Initialization and teardown
//
// Initialize the ECS
bool ecs_start(void) {
  resize(POOL_SIZE);

  if(!entities) {
    error = "Could not allocate entities";
    return false;
  }

  return true;
}


// Destroy the ECS and free all memory
void ecs_stop(void) {
  for(int i = 0; i < num_entities; i++) {
    if(!entities[i].alive && !entities[i].immortal)
      continue;

    for(int j = 0; j < NUM_COMPONENTS; j++) {
      if(entities[i].component_mask & ((EcsMask)1<<j))
        free(entities[i].components[j]);
    }
  }

  num_entities = 0;
  free(entities);
  entities = NULL;
}


//
// Entity management
//
// Create a new entity and return its ID
EcsID ecs_create_entity(void) {
  int idx = 0;
  for(;
      idx < num_entities && (entities[idx].alive || entities[idx].immortal);
      idx++);
  
  if(idx == num_entities)
    resize(num_entities*2);
  
  entities[idx].version++;
  entities[idx].alive = true;

  return (EcsID){.id=idx, .version=entities[idx].version};
}


// Reincarnate an entity
void ecs_reincarnate(EcsID ent) {
  if(!ecs_is_valid(ent) || entities[ent.id].alive || !entities[ent.id].immortal)
    return;

  entities[ent.id].alive = true;
}


// Set an entity to be immortal. Immortal entities can be reincarnated.
void ecs_set_immortal(EcsID ent, bool immortality) {
  if(!ecs_is_valid(ent))
    return;

  entities[ent.id].immortal = immortality;
}

// Returns whether an entity is immortal
bool ecs_is_immortal(EcsID ent) {
  return ecs_is_valid(ent) && entities[ent.id].immortal;
}

// Destroy an entity
void ecs_kill_entity(EcsID ent) {
  if(!ecs_is_valid(ent))
    return;

  entities[ent.id].alive = false;
  if(entities[ent.id].immortal)
    return;

  entities[ent.id].version++;
  for(int i = 0; i < NUM_COMPONENTS; i++)
    if((EcsMask)1 << i & entities[ent.id].component_mask) {
      free(entities[ent.id].components[i]);
      entities[ent.id].components[i] = 0;
    }
}


//
// Component management
//
// Add a component
void *ecs_add_component(EcsID ent, int id, size_t size) {
  EcsMask mask = 1 << id;
  if(!ecs_is_valid(ent) || (entities[ent.id].component_mask & mask) != 0)
    return NULL;

  entities[ent.id].component_mask |= mask;
  
  void *component = calloc(size, 1);
  entities[ent.id].components[id] = component;
  return component;
}


// Remove a component
void ecs_remove_component(EcsID ent, int id) {
  EcsMask mask = 1 << id;
  if(!ecs_is_valid(ent) || (entities[ent.id].component_mask & mask) == 0)
    return;

  entities[ent.id].component_mask &= ~mask;
  free(entities[ent.id].components[id]);
}


// Does this entity have this component?
bool ecs_has_component(EcsID ent, int id) {
  return ecs_is_valid(ent) && !!(entities[ent.id].component_mask & (1<<id));
}


// Get a component, returns NULL on error
void *ecs_get_component(EcsID ent, int id) {
  EcsMask mask = 1 << id;
  if(!ecs_is_valid(ent) || !(entities[ent.id].component_mask & mask))
    return NULL;
  return entities[ent.id].components[id];
}


//
// Flag management
//
// Does the entity have this flag?
bool ecs_has_flags(EcsID ent, EcsMask fmask) {
  return ecs_is_valid(ent)
    && !!(entities[ent.id].flag_mask & fmask);
}

// Set flag on entity
void ecs_set_flags(EcsID ent, EcsMask fmask) {
  if(ecs_is_valid(ent))
    entities[ent.id].flag_mask |= fmask;
}

// Clear flag on entity
void ecs_clear_flags(EcsID ent, EcsMask fmask) {
  if(ecs_is_valid(ent))
    entities[ent.id].flag_mask &= ~fmask;
}


//
// Iteration
//
// Iterate over all entities. If the entity has all the
// components in cmask and all the flags in fmask, then
// call the function func.
void ecs_iterate(EcsMask cmask, EcsMask fmask, void (*func)(EcsID)) {
  for(int i = 0; i < num_entities; i++) {
    if(entities[i].alive
        && (entities[i].component_mask & cmask) == cmask
        && (entities[i].flag_mask & fmask) == fmask
      ) {
      func((EcsID){.id=i, .version=entities[i].version});
    }
  }
}


//
// Searching
//
// Find the first entity with matching flag and component mask
EcsID ecs_find(EcsMask cmask, EcsMask fmask) {
  for(int i = 0; i < num_entities; i++) {
    if(entities[i].alive
        && (entities[i].component_mask & cmask) == cmask
        && (entities[i].flag_mask & fmask) == fmask
      ) {
      return (EcsID){.id=i, .version=entities[i].version};
    }
  }

  return (EcsID){.id=-1};
}
