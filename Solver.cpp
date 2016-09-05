//
// Created by awbrenn on 8/31/16.
//

#include "Solver.h"


Solver::Solver(float box_width, float box_height, float box_depth, Vector3d ball_position, float ball_radius, float H) {
  box = new Box(box_width, box_height, box_depth);
  ball = new Ball(ball_position, ball_radius);
  h = H;
}

int Solver::detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, float *s) {
  int collision_wall = -1; // -1 indicates no collision
  bool collision_occurred = false;
  Vector3d wall_location;
  Vector3d wall_normal;
  float distance_to_wall, total_distance;
  float min_s = 2.0f;
  float temp_s;
  float epsilon = 0.0000001; // used to avoid division by zero

  for (int i=0; i < 6; ++i) {
    wall_location = box->wall_locations[i];
    wall_normal = box->wall_normals[i];


    /* TODO Fix issues with box walls. I'm not storing the information correctly
       in the box constructor. */
    distance_to_wall = (float) ((ball_pos - wall_location) * wall_normal);
    total_distance = (float) ((ball_pos - new_ball_pos) * wall_normal);

    if (abs(total_distance) < epsilon) // avoid division by zero
      continue;

    temp_s = distance_to_wall / total_distance;
    if (temp_s >= 0.0f && temp_s < 1.0f) {
      collision_occurred = true;
      collision_wall = i;
      if (temp_s < min_s)
        min_s = temp_s;
    }
  }

  if (collision_occurred)
    *s = min_s;

  return collision_wall;
}

void Solver::update() {
  Vector3d pos_new, pos_collision,
           velocity_new, velocity_collison,
           velocity_normal, velocity_tangent;
  int collision_wall;
  float s; // fraction between distance of wall to ball
           // and total distance travelled in normal direction


  velocity_new = ball->velocity + acceleration * h;
  pos_new = ball->pos + velocity_new * h;

  collision_wall = detectCollision(ball->pos, pos_new, &s);
  if (collision_wall != -1) { // collision occurred!
    velocity_collison = ball->velocity + s * h * acceleration;
    pos_collision = ball->pos + s * h * ball->velocity;
    velocity_normal = (velocity_collison * box->wall_normals[collision_wall]) * box->wall_normals[collision_wall];
    velocity_tangent = velocity_collison - velocity_normal;
    velocity_normal = -1.0f * velocity_normal; // TODO add Cr
    velocity_tangent = (1.0f - 0.0f) * velocity_tangent; // TODO replace 0.0f with Cf
    velocity_collison = velocity_normal + velocity_tangent;
    ball->velocity = velocity_collison + (1.0f - s) * h * acceleration;
    ball->pos = pos_collision + (1.0f - s) * h * velocity_collison;
  }
  else {
    ball->pos = pos_new;
    ball->velocity = velocity_new;
  }
}
