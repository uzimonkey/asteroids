#include "vid.h"
#include <SDL.h>

#define CUTE_PNG_IMPLEMENTATION
#include "cute_png.h"


int num_textures;
struct texture {
  char *filename;
  SDL_Texture *tex;
  int w, h;
} *textures;


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RGBA_MASK 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
#define RGBA_MASK 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif


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
// Loading assets
//
// Find an already-loaded texture
TextureID vid_find_loaded_texture(const char *filename) {
  int idx;
  for(idx = 0; idx < num_textures && strcmp(textures[idx].filename, filename););
  if(idx == num_textures)
    return -1;
  return idx;
}

// Load a texture and return a pointer to it
TextureID vid_load_texture(const char *filename) {
  cp_image_t img = {0};
  SDL_Surface* surf = NULL;

  int idx;

  idx = vid_find_loaded_texture(filename);
  if(idx != -1)
    return idx;

  for(idx = 0; idx < num_textures && textures[idx].tex != NULL;);
  if(idx == num_textures) {
    num_textures = num_textures == 0 ? 32 : num_textures * 2;
    textures = realloc(textures, sizeof(*textures) * num_textures);
    if(textures == NULL) {
      error = "Could not allocate texture table space";
      idx = -1;
      goto done;
    }
  }
  
  img = cp_load_png(filename);
  if(img.pix == NULL) {
    error = cp_error_reason;
    idx = -1;
    goto done;
  }

  surf = SDL_CreateRGBSurfaceFrom(
      img.pix, img.w, img.h, 32, img.w*4, RGBA_MASK);
  if(surf == NULL) {
    error = SDL_GetError();
    idx = -1;
    goto done;
  }

  textures[idx].tex = SDL_CreateTextureFromSurface(renderer, surf);
  if(textures[idx].tex == NULL) {
    error = SDL_GetError();
    idx = -1;
    goto done;
  }

  textures[idx].w = surf->w;
  textures[idx].h = surf->h;

done:
  SDL_FreeSurface(surf);
  free(img.pix);
  return idx;
}


// Free a texture
void vid_free_texture(TextureID tex) {
  if(tex < 0 || tex > num_textures || textures[tex].tex == NULL)
    return;

  SDL_DestroyTexture(textures[tex].tex);
  free(textures[tex].filename);
  textures[tex].tex = NULL;
  textures[tex].filename = NULL;
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

// Draw a sprite
void vid_draw_sprite(TextureID tid, float x, float y) {
  if(tid < 0 || tid > num_textures || textures[tid].tex == NULL)
    return;

  SDL_RenderCopy(
      renderer,
      textures[tid].tex,
      NULL,
      &(SDL_Rect){
        .x = x - textures[tid].w/2,
        .y = y - textures[tid].h/2,
        .w = textures[tid].w,
        .h = textures[tid].h
      });
}

