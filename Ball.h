//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_BALL_H
#define BOUNCING_BALL_BALL_H

#include "Matrix.h"

class Ball {
  public:
    Ball(Vector3d pos, float radius);

    Vector3d pos;
    Vector3d velocity;
    float radius;
};


#endif //BOUNCING_BALL_BALL_H
