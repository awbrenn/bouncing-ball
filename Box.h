//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_BOX_H
#define BOUNCING_BALL_BOX_H

#include <vector>
#include "Matrix.h"

class Box {
  public:
    std::vector<Vector3d> wall_locations;
    std::vector<Vector3d> wall_normals;

    Box(float width, float height, float depth);
};


#endif //BOUNCING_BALL_BOX_H
