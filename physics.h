#ifndef PHYSICS_H
#define PHYSICS_H

bool physics_start(void);
void physics_stop(void);
const char *physics_get_error(void);

void physics_sys(double delta_time);

#endif
