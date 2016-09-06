//
// Created by awbrenn on 8/31/16.
//

#include <cfloat>
#include "Solver.h"


Solver::Solver(double box_width, double box_height, double box_depth,
               Vector3d ball_position, double ball_radius, double ball_mass, double H,
               int Substeps, double coeff_restitution, double coeff_friction,
               double coeff_air_resistance) {
  box = new Box(box_width, box_height, box_depth);
  ball = new Ball(ball_position, ball_radius, ball_mass);
  h = H;
  substeps = Substeps;
  cr = coeff_restitution;
  cf = coeff_friction;
  ar = coeff_air_resistance;
  acceleration = gravity;
}


int Solver::detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, double *s) {
  int collision_wall = -1; // -1 indicates no collision
  Vector3d wall_location;
  Vector3d wall_normal;
  Vector3d point_on_ball;
  Vector3d point_on_new_ball;
  double distance_to_wall, total_distance;
  double min_s = 2.0f; // set min_s to a number arbitrarilly greater than 1.0
  double temp_s;
  double epsilon = 0.00000001f; // used to avoid division by zero

  for (int i=0; i < 6; ++i) {
    wall_location = box->wall_locations[i];
    wall_normal = box->wall_normals[i];

    // get the point on ball surface closest to the plane
    point_on_ball = -1.0f * wall_normal * ball->radius + ball_pos;
    point_on_new_ball = -1.0f * wall_normal * ball->radius + new_ball_pos;

    distance_to_wall = (point_on_ball - wall_location) * wall_normal;
    total_distance = (point_on_ball - point_on_new_ball) * wall_normal;

    if (abs(total_distance) < epsilon) // avoid division by zero
      continue;

    // find the first collision to occur
    temp_s = distance_to_wall / total_distance;
    if (temp_s >= 0.0f && temp_s < 1.0f && temp_s < min_s) {
        *s = temp_s;
        collision_wall = i;
        min_s = temp_s;
    }
  }

  return collision_wall;
}

// TODO add multiple collision reaction.
void Solver::update() {
  Vector3d pos_new, pos_collision,
           velocity_new, velocity_collison,
           velocity_normal, velocity_tangent;
  int collision_wall;
  double s; // fraction between distance of wall to ball
           // and total distance travelled in normal direction

  acceleration = gravity - ((ar/ball->mass) * ball->velocity);
  velocity_new = ball->velocity + acceleration * h;
  pos_new = ball->pos + velocity_new * h;

  collision_wall = detectCollision(ball->pos, pos_new, &s);
  if (collision_wall != -1) { // collision occurred!
    velocity_collison = ball->velocity + s * h * acceleration;
    pos_collision = ball->pos + s * h * ball->velocity;
    velocity_normal = (velocity_collison * box->wall_normals[collision_wall]) * box->wall_normals[collision_wall];
    velocity_tangent = velocity_collison - velocity_normal;
    velocity_normal = -1.0 * cr * velocity_normal;
    velocity_tangent = (1.0f - cf) * velocity_tangent;
    velocity_collison = velocity_normal + velocity_tangent;
    acceleration = gravity - ((ar/ball->mass) * velocity_collison);
    ball->velocity = velocity_collison + (1 - s) * h * acceleration;
    ball->pos = pos_collision + (1.0f - s) * h * velocity_collison;
  }
  else {
    ball->pos = pos_new;
    ball->velocity = velocity_new;
  }
}
