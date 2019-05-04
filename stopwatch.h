#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <SDL_timer.h>

typedef Uint64 Stopwatch;

Stopwatch stopwatch_start();
double stopwatch_time(Stopwatch s);

#endif
