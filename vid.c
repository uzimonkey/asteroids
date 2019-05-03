#include "vid.h"
#include <SDL.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static const char *error = "";

//
// Utility
//
// Return the last error
const char *vid_get_error(void) {
  return error;
}

//
// Start and Stop
//
// Start the vid module
bool vid_start(void) {
  if(SDL_InitSubSystem(SDL_INIT_VIDEO))
    goto error;

  window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_RESIZABLE);
  if(!window)
    goto error;

  renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!renderer)
    goto error;

  return true;

error:
  error = SDL_GetError();
  return false;
}

// Stop the vid module
void vid_stop(void) {
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


//
// Drawing
//
// Begin a frame. Clears the screen and gets ready for rendering
void vid_begin_frame(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

// End a frame. This pushes it to the device and may wait for vsync if enabled.
void vid_end_frame(void) {
  SDL_RenderPresent(renderer);
}

// Set a render color for future calls to render objects
void vid_set_color(Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

// Draw the outline of a rectangle
void vid_draw_rect(float x, float y, float w, float h) {
  SDL_RenderDrawRect(renderer, &(SDL_Rect){.x=x, .y=y, .w=w, .h=h});
}
