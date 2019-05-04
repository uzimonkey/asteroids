#include "stopwatch.h"

Stopwatch stopwatch_start() {
  return SDL_GetPerformanceCounter();
}

double stopwatch_time(Stopwatch s) {
  return (SDL_GetPerformanceCounter() - s)
    / (double)SDL_GetPerformanceFrequency();
}
