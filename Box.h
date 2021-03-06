//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_BOX_H
#define BOUNCING_BALL_BOX_H

#include <vector>
#include "Matrix.h"

class Box {
  public:
    double w; // width
    double h; // height
    double d; // depth
    std::vector<Vector3d> wall_locations;
    std::vector<Vector3d> wall_normals;

    Box(double width, double height, double depth);
};


#endif //BOUNCING_BALL_BOX_H
