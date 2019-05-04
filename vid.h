#ifndef VID_H
#define VID_H
#include <stdbool.h>
#include <stdint.h>

#define WINDOW_TITLE "Asteroids"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SCENE_WIDTH 640
#define SCENE_HEIGHT 480


//
// Types
//
typedef struct {
  float x, y;
  float w, h;
} Rect;

typedef struct {
  uint8_t r, g, b, a;
} Color;

typedef int TextureID;


//
// Utility
//
// Return the last error
const char *vid_get_error(void);


//
// Loading assets
//
// Find an already-loaded texture
TextureID vid_find_loaded_texture(const char *filename);

// Load a texture and return a pointer to it
TextureID vid_load_texture(const char *filename);

// Free a texture
void vid_free_texture(TextureID tex);


//
// Start and Stop
//
// Start the vid module
bool vid_start(void);

// Stop the vid module
void vid_stop(void);


//
// Drawing
//
// Begin a frame. Clears the screen and gets ready for rendering
void vid_begin_frame(void);

// End a frame. This pushes it to the device and may wait for vsync if enabled.
void vid_end_frame(void);

// Set a render color for future calls to render objects
void vid_set_color(Color c);

// Draw the outline of a rectangle
void vid_draw_rect(float x, float y, float w, float h);

// Draw a sprite
void vid_draw_sprite(TextureID tid, float x, float y);



#endif
