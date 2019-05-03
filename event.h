#ifndef EVENT_H
#define EVENT_H
#include <stdbool.h>
#include <SDL_scancode.h>

//
// Utility
//
// Get the last error
const char *event_get_error(void);


//
// Start and stop
//
// Start the event module
bool event_start(void);

// Stop the event module
void event_stop(void);


//
// Keyboard
//
// Return true if the key is currently being pressed
bool event_key_down(int k);
#define event_key_down(k) event_key_down(SDL_SCANCODE_##k)

// Return true if the key was pressed this frame
bool event_key_pressed(int k);
#define event_key_pressed(k) event_key_pressed(SDL_SCANCODE_##k)


//
// Event processing
//
// Process waiting events
bool event_process(void);

#endif
