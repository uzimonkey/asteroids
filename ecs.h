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
  float x;
  float y;
} POS;


typedef struct {
  float x;
  float y;
} VEL;

enum { 
  COMPONENT_POS,
  COMPONENT_VEL,
  NUM_COMPONENTS
};

enum {
  FLAG_PLAYER,
  FLAG_ENEMY,
  NUM_FLAGS
};


//
// Functions
//
// Initialize the ECS
void ecs_init(void);

// Create a new entity and return its ID
EcsID ecs_create_entity(void);

// Destroy an entity
void ecs_destroy_entity(EcsID id);

// Is this EcsID valid?
extern inline bool is_valid(EcsID ent);

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
