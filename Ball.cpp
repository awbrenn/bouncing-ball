//
// Created by awbrenn on 8/31/16.
//

#include "Ball.h"

Ball::Ball(Vector3d position, float radius) {
  pos = position;
  velocity = Vector3d(0.0f, 0.0f, 0.0f);
  radius = radius;
}
