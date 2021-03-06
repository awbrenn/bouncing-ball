//
// Author: Austin Brennan
// Date: September 5th, 2016
//
#include "Camera.h"
#include "Solver.h"
#include <cstdio>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int WIDTH = 1280;
int HEIGHT = 720;

int persp_win;

Camera *camera;
Solver *solver;

bool showGrid = false;

// draws a simple grid
void makeGrid() {
  glColor3f(0.0f, 0.0f, 0.0f);

  glLineWidth(1.0f);

  for (double i=-12; i<12; i++) {
    for (double j=-12; j<12; j++) {
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j+1);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i+1, 0, j);
      glEnd();

      if (j == 11){
	glBegin(GL_LINES);
	glVertex3f(i, 0, j+1);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
      if (i == 11){
	glBegin(GL_LINES);
	glVertex3f(i+1, 0, j);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
    }
  }

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(-12, 0, 0);
  glVertex3f(12, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0, 0, -12);
  glVertex3f(0, 0, 12);
  glEnd();
  glLineWidth(1.0f);
}

// TODO Base this function off of the azim and elev of camera.
void getVisibleFaces(bool *visible_faces, std::vector<Vector3d> face_positions) {
  // set max distance to camera to the distance from the front face to the camera
  double test_distance_to_camera;
  double min_distance_to_camera = (face_positions[0] - camera->Pos).norm();
  int remove_face = 0;

  for (int i=1; i < 6; ++i) {
    test_distance_to_camera = (face_positions[i] - camera->Pos).norm();
    if (test_distance_to_camera < min_distance_to_camera) {
      min_distance_to_camera = test_distance_to_camera;
      remove_face = i;
    }
  }

  *(visible_faces + remove_face) = false;
}

void makeBox() {
  bool visible_faces[6] = {true, true, true, true, true, true};
  getVisibleFaces(visible_faces, solver->box->wall_locations);
  float w = (float) solver->box->w / 2.0f;
  float h = (float) solver->box->h / 2.0f;
  float d = (float) solver->box->d / 2.0f;

  if (visible_faces[0]) {
    // back face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }

  if (visible_faces[1]) {
    // front face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -1.0f * h, d);
    glVertex3f(w, h, d);
    glVertex3f(-1.0f * w, h, d);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glEnd();
  }

  if (visible_faces[2]) {
    // right face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(w, h, d);
    glVertex3f(w, -1.0f * h, d);
    glEnd();
  }

  if (visible_faces[3]) {
    // left face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glVertex3f(-1.0f * w, h, d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }

  if (visible_faces[4]) {
    // top face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, d);
    glEnd();
  }

  if (visible_faces[5]) {
    // bottom face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, -1.0f * h, d);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }
}

void initCamera() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 5), Vector3d(0, 0, 0),
		      Vector3d(0, 1, 0));

  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0f);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initSimulation(char *param_filename) {
  FILE *paramfile;
  double box_width, box_height, box_depth,
         ball_x, ball_y, ball_z, ball_radius, ball_mass,
         timestep, coeff_restitution, coeff_friction,
         coeff_air_resistance;
  int substeps;
  char skipline_buffer[1024];
  char *skipline;

  if((paramfile = std::fopen(param_filename, "r")) == NULL) {
    fprintf(stderr, "error opening parameter file %s", param_filename);
    exit(1);
  }

  fgets(skipline_buffer, 1024, paramfile);
  if(fscanf(paramfile, "%lf %lf %lf",
            &box_width, &box_height, &box_depth) != 3){
    fprintf(stderr, "error reading parameter file %s\n", param_filename);
    exit(1);
  }
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  if(fscanf(paramfile, "%lf %lf %lf %lf %lf",
            &ball_x, &ball_y, &ball_z, &ball_radius, &ball_mass) != 5){
    fprintf(stderr, "error reading parameter file %s\n", param_filename);
    exit(1);
  }
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  if(fscanf(paramfile, "%lf %d",
            &timestep, &substeps) != 2){
    fprintf(stderr, "error reading parameter file %s\n", param_filename);
    exit(1);
  }
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  fgets(skipline_buffer, 1024, paramfile);
  if(fscanf(paramfile, "%lf %lf %lf",
            &coeff_restitution, &coeff_friction, &coeff_air_resistance) != 3){
    fprintf(stderr, "error reading parameter file %s\n", param_filename);
    exit(1);
  }

  fclose(paramfile);

  solver = new Solver(box_width, box_height, box_depth,
                      Vector3d(ball_x, ball_y, ball_z),
                      ball_radius, ball_mass, timestep, substeps,
                      coeff_restitution, coeff_friction, coeff_air_resistance);
}

void simulateBall() {
  for(int i = 0; i < solver->substeps; ++i) {
    solver->update();
  }
  glutPostRedisplay();
}


void perspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (showGrid)
    makeGrid();

  makeBox();

  glColor3f(1,0,0.5);
  GLUquadric *quad;
  quad = gluNewQuadric();
  glTranslatef(solver->ball->pos.x,solver->ball->pos.y,solver->ball->pos.z);
  gluSphere(quad,solver->ball->radius,100,20);

  glFlush();
  glutSwapBuffers();
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void keyboardEventHandler(unsigned char key, int x, int y) {
  switch (key) {
  case 'r': case 'R':
    // reset the camera to its initial position
    camera->Reset();
    break;

  case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;

  case 'g': case 'G':
    showGrid = !showGrid;
    break;

  case 'w':
    solver->gravity = {0.0f, 9.8f, 0.0f};
    cout << "Gravity is now up" << endl;
    break;

  case 'a':
    solver->gravity = {-9.8f, 0.0f, 0.0f};
    cout << "Gravity is now left" << endl;
    break;

  case 's':
    solver->gravity = {0.0f, -9.8f, 0.0f};
    cout << "Gravity is now down" << endl;
    break;

  case 'd':
    solver->gravity = {9.8f, 0.0f, 0.0f};
    cout << "Gravity is now right" << endl;
    break;

  case 'z':
    solver->gravity = {0.0f, 0.0f, 9.8f};
    cout << "Gravity is now forward" << endl;
    break;

  case 'x':
    solver->gravity = {0.0f, 0.0f, -9.8f};
    cout << "Gravity is now back" << endl;
    break;

  case 'q': case 'Q':	// q or esc - quit
  case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}

int main(int argc, char *argv[]) {

  if(argc != 2){
    fprintf(stderr, "usage: BouncingBall paramfile\n");
    exit(1);
  }

  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Ball Bouncing in 6 Sided Box");

  // initialize the camera and such
  initCamera();

  initSimulation(argv[1]);

  // set up opengl callback functions
  glutDisplayFunc(perspDisplay);
  glutIdleFunc(simulateBall);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);


  glutMainLoop();
  return(0);
}

