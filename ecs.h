#ifndef ECS_H
#define ECS_H

#include "vid.h"
#include "mathlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//
// Types
//
// The mask determines the maximum number of component types
// and flags
typedef uint32_t EcsMask;

// An EcsID is returned by the ECS to refer to an entity
typedef struct {
  int id;
  int version;
} EcsID;


//
// Constants
//
// The POOL_SIZE determines the maximum number of entities that
// the ecs will initially allocate. If this number is exceeded,
// the pool will be reallocated.
#define POOL_SIZE 128

//
// Components
//
typedef struct {
  Vec2 pos;
  float rot;
} TRANSFORM;

typedef struct {
  Vec2 vel;
  float drag;
  float angular_vel;
  float angular_drag;
} PHYSICS;

typedef TextureID TEX;

#define COMPONENT_NONE 0
enum { 
  COMPONENT_TRANSFORM,
  COMPONENT_PHYSICS,
  COMPONENT_TEX,
  NUM_COMPONENTS
};

#define FLAG_NONE 0
enum {
  FLAG_PLAYER,
  NUM_FLAGS
};


//
// Utility Functions
//
// Is this EcsID valid?
bool ecs_is_valid(EcsID ent);

// Convert and ID to a mask
#define ecs_id_to_mask(id) ((EcsMask)1 << (id))
#define ecs_component_mask(type)\
  ecs_id_to_mask(COMPONENT_##type)
#define ecs_flag_mask(type)\
  ecs_id_to_mask(FLAG_##type)

// Get the last error
const char *ecs_get_error(void);


//
// Initialization and teardown
//
// Initialize the ECS
bool ecs_start(void);

// Destroy the ECS and free all memory
void ecs_stop(void);


//
// Entity management
//
// Create a new entity and return its ID
EcsID ecs_create_entity(void);

// Reincarnate an entity
void ecs_reincarnate(EcsID ent);

// Set an entity to be immortal. Immortal entities can be reincarnated.
void ecs_set_immortal(EcsID ent, bool immortalit);

// Returns whether an entity is immortal
bool ecs_is_immortal(EcsID ent);

// Kill an entity
void ecs_kill_entity(EcsID ent);


//
// Component management
//
// Add a component
void *ecs_add_component(EcsID ent, int id, size_t size);
#define ecs_add_component(ent, type)\
  ecs_add_component(ent, COMPONENT_##type, sizeof(type))

// Remove a component
void ecs_remove_component(EcsID ent, int id);
#define ecs_remove_component(ent, type)\
  ecs_remove_component(ent, COMPONENT_##type)

// Does this entity have this component?
bool ecs_has_component(EcsID ent, int id);
#define ecs_has_component(ent, type)\
  ecs_has_component(ent, COMPONENT_##type)

// Get a component, returns NULL on error
void *ecs_get_component(EcsID ent, int id);
#define ecs_get_component(ent, type)\
  ecs_get_component(ent, COMPONENT_##type)


//
// Flag management
//
// Does the entity have this flag?
bool ecs_has_flags(EcsID ent, EcsMask fmask);
#define ecs_has_flag(ent, flag)\
  ecs_has_flags(ent, ecs_flag_mask(flag))

// Set flag on entity
void ecs_set_flags(EcsID ent, EcsMask fmask);
#define ecs_set_flag(ent, flag)\
  ecs_set_flags(ent, ecs_flag_mask(flag))

// Clear flag on entity
void ecs_clear_flags(EcsID ent, EcsMask fmask);
#define ecs_clear_flag(ent, flag)\
  ecs_clear_flags(ent, ecs_flag_mask(flag))


//
// Iteration
//
// Iterate over all entities. If the entity has all the
// components in cmask and all the flags in fmask, then
// call the function func.
void ecs_iterate(EcsMask cmask, EcsMask fmask, void (*func)(EcsID));
#define ecs_iterate_component(comp, func)\
  ecs_iterate(ecs_component_mask(comp), 0, func);
#define ecs_iterate_flag(flag, func)\
  ecs_iterate(0, ecs_flag_mask(flag), func);


//
// Searching
//
// Find the first entity with matching flag and component mask
EcsID ecs_find(EcsMask cmask, EcsMask fmask);
#define ecs_find_component(comp)\
  ecs_find(ecs_component_mask(comp), 0);
#define ecs_find_flag(flag)\
  ecs_find(0, ecs_flag_mask(flag));

#endif
