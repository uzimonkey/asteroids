#ifndef VID_H
#define VID_H
#include <stdbool.h>
#include <stdint.h>

#define WINDOW_TITLE "Asteroids"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define VIRTUAL_WIDTH 320
#define VIRTUAL_HEIGHT 240


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


//
// Utility
//
// Return the last error
const char *vid_get_error(void);


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
void vid_draw_rect(Rect r);



#endif
