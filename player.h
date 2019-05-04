#ifndef PLAYER_H
#define PLAYER_H

#define SPRITE "playerShip3_red.png"

#define TORQUE 540.0
#define MAX_ANGULAR_VEL 360

#define THRUST 100

#define ANGULAR_DRAG 0.3
#define ANGULAR_DRAG_SENSITIVITY 0.05

#define DRAG 0.1
#define DRAG_SENSITIVITY 0.1

void player_spawn(void);
void player_sys(float delta_time);

#endif
