//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_SOLVER_H
#define BOUNCING_BALL_SOLVER_H
#include "Ball.h"
#include "Box.h"

class Solver {
  private:
  int detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, float *s);

  public:
    Ball *ball;
    Box *box;
    float h; // timestep
    Vector3d acceleration = Vector3d(0.0f, -9.8f, 0.0f);

    // constructors
    Solver(float box_width, float box_height, float box_depth, Vector3d ball_position, float ball_radius, float h);

    // member functions
    void update();
};


#endif //BOUNCING_BALL_SOLVER_H
