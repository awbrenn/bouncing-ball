//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_SOLVER_H
#define BOUNCING_BALL_SOLVER_H
#include "Ball.h"
#include "Box.h"

class Solver {
  private:
  int detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, double *s);

  public:
    Ball *ball;
    Box *box;
    double h; // timestep
    int substeps;
    Vector3d gravity = Vector3d(0.0f, -9.8f, 0.0f);
    Vector3d acceleration = Vector3d(0.0f, 0.0f, 0.0f);
    double cr; // coefficient of restitution
    double cf; // coefficient of friction
    double ar; // air resistance

    // constructors
    Solver(double box_width, double box_height, double box_depth,
           Vector3d ball_position, double ball_radius, double ball_mass,
           double h, int substeps, double cr, double cf, double ar);

    // member functions
    void update();
};


#endif //BOUNCING_BALL_SOLVER_H
