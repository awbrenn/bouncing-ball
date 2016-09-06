//
// Created by awbrenn on 8/31/16.
//

#include "Box.h"

Box::Box(double width, double height, double depth) {
  w = width;
  h = height;
  d = depth;
  width /= 2;
  height /= 2;
  depth /= 2;


  wall_locations = {Vector3d( 0.0f, 0.0f, -1.0 * depth),    // back face
                    Vector3d( 0.0f, 0.0f, depth),           // front face
                    Vector3d( width, 0.0f, 0.0f),           // right face
                    Vector3d(-1.0f * width, 0.0f, 0.0f),    // left face
                    Vector3d( 0.0f, height, 0.0f),          // top face
                    Vector3d( 0.0f, -1.0f * height, 0.0f)}; // bottom face

  wall_normals = {Vector3d( 0.0f,  0.0f,  1.0f),  // back face
                  Vector3d( 0.0f,  0.0f, -1.0f),  // front face
                  Vector3d(-1.0f,  0.0f,  0.0f),  // right face
                  Vector3d( 1.0f,  0.0f,  0.0f),  // left face
                  Vector3d( 0.0f, -1.0f,  0.0f),  // top face
                  Vector3d( 0.0f,  1.0f,  0.0f)}; // bottom face
}