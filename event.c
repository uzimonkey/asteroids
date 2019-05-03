#include "event.h"
#include <SDL.h>
#include <stdlib.h>
#include <string.h>

// Unwrap
#undef event_key_down
#undef event_key_pressed

static int frame;

static struct {
  bool down;
  int last_pressed;
} keys[SDL_NUM_SCANCODES];

static const char *error = "";


//
// Utility
//
// Get the last error
const char *event_get_error(void) {
  return error;
}


//
// Start and stop
//
// Start the event module
bool event_start(void) {
  if(SDL_InitSubSystem(SDL_INIT_EVENTS) == -1) {
    error = SDL_GetError();
    return false;
  }

  memset(keys, 0, sizeof(keys));

  return true;
}

// Stop the event module
void event_stop(void) {
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}


//
// Keyboard
//
// Return true if the key is currently being pressed
bool event_key_down(int k) {
  return k >= 0 && k < SDL_NUM_SCANCODES && keys[k].down;
}

// Return true if the key was pressed this frame
bool event_key_pressed(int k) {
  return k >= 0 && k < SDL_NUM_SCANCODES && keys[k].last_pressed == frame;
}


//
// Event processing
//
// Handle keyboard event
static void keyboard(SDL_KeyboardEvent key) {
  if(key.repeat)
    return;

  keys[key.keysym.scancode].down = key.type == SDL_KEYDOWN;
  if(key.type == SDL_KEYDOWN)
    keys[key.keysym.scancode].last_pressed = frame;
}

// Handle mouse event
static void mouse_motion(SDL_MouseMotionEvent mm) {
}

static void mouse_button(SDL_MouseButtonEvent mb) {
}

static void mouse_wheel(SDL_MouseWheelEvent mw) {
}

// Process waiting events
bool event_process(void) {
  frame++;

  bool wants_quit = false;
  for(SDL_Event e; SDL_PollEvent(&e);) {
    switch(e.type) {
    case SDL_QUIT:
      wants_quit = true;
      break;

    // Keyboard
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      keyboard(e.key);
      break;

    // Mouse
    case SDL_MOUSEMOTION:
      mouse_motion(e.motion);
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      mouse_button(e.button);
      break;
    case SDL_MOUSEWHEEL:
      mouse_wheel(e.wheel);
      break;
    }
  }

  return wants_quit;
}


