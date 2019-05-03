#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//
// Types
//
// The mask determines the maximum number of component types
// and flags
typedef uint32_t EcsMask;

typedef struct {
  float x;
  float y;
} Vec2f;

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
typedef Vec2f POS;
typedef Vec2f VEL;

#define COMPONENT_NONE 0
enum { 
  COMPONENT_POS,
  COMPONENT_VEL,
  NUM_COMPONENTS
};

#define FLAG_NONE 0
enum {
  FLAG_PLAYER,
  FLAG_ENEMY,
  NUM_FLAGS
};


//
// Utility Functions
//
// Is this EcsID valid?
bool ecs_is_valid(EcsID ent);

// Convert and ID to a mask
EcsMask ecs_id_to_mask(int id);
#define ecs_component_type_to_mask(type)\
  ecs_id_to_mask(COMPONENT_##type)
#define ecs_flag_id_to_mask(type)\
  ecs_id_to_mask(FLAG_##type)


//
// Initialization and teardown
//
// Initialize the ECS
void ecs_start(void);

// Destroy the ECS and free all memory
void ecs_stop(void);


//
// Entity management
//
// Create a new entity and return its ID
EcsID ecs_create_entity(void);

// Destroy an entity
void ecs_destroy_entity(EcsID id);


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
bool ecs_has_flag(EcsID ent, int id);

// Set flag on entity
void ecs_set_flag(EcsID ent, int id);

// Clear flag on entity
void ecs_clear_flag(EcsID ent, int id);


//
// Iteration
//
// Iterate over all entities. If the entity has all the
// components in cmask and all the flags in fmask, then
// call the function func.
void ecs_iterate(EcsMask cmask, EcsMask fmask, void (*func)(EcsID));
#define ecs_iterate_component(comp, func)\
  ecs_iterate(ecs_component_type_to_mask(comp), 0, func);
#define ecs_iterate_flag(flag, func)\
  ecs_iterate(0, ecs_flag_id_to_mask(flag), func);


#endif
