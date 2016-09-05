//
// CameraExample.cpp
//
// Example program to show how to use Chris Root's OpenGL Camera Class
// 
// Christopher Root, 2006
// Minor Modifications by Donald House, 2009
// Minor Modifications by Yujie Shu, 2012
//
#include "Camera.h"
#include "Solver.h"

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int WIDTH = 640;
int HEIGHT = 480;

int persp_win;

Camera *camera;
Solver *solver;

bool showGrid = true;

// draws a simple grid
void makeGrid() {
  glColor3f(0.0f, 0.0f, 0.0f);

  glLineWidth(1.0f);

  for (float i=-12; i<12; i++) {
    for (float j=-12; j<12; j++) {
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

  if (visible_faces[0]) {
    // back face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glEnd();
  }

  if (visible_faces[1]) {
    // front face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, 2.0f);
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    glEnd();
  }

  if (visible_faces[2]) {
    // right face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(2.0f, -2.0f, 2.0f);
    glEnd();
  }

  if (visible_faces[3]) {
    // left face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-2.0f, -2.0f, 2.0);
    glVertex3f(-2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, 2.0, -2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glEnd();
  }

  if (visible_faces[4]) {
    // top face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, 2.0f);
    glEnd();
  }

  if (visible_faces[5]) {
    // bottom face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    glVertex3f(2.0f, -2.0f, 2.0f);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
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

void initSimulation() {
  solver = new Solver(4.0f, 4.0f, 4.0f, Vector3d(0.0f, 0.0f, 0.0f), 0.2f, 0.1f);
}

void simulateBall() {
  solver->update();
  glutPostRedisplay();
}

void perspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);



//  cout << camera->Aim.x << " " << camera->Aim.y << " " << camera->Aim.z << endl;
//  cout << camera->TranslateX << " " << camera->TranslateY << endl;
//  cout << camera->CurrentAzim << " " << camera->CurrentElev << endl;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (showGrid)
    makeGrid();

  makeBox();

  glPointSize(10.0f);
  glBegin(GL_POINTS);
  glColor3f(1.0f, 0.5f, 1.0f);
  glVertex3f(solver->ball->pos.x, solver->ball->pos.y, solver->ball->pos.z);
  glEnd();

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
    solver->acceleration = {0.0f, 9.8f, 0.0f};
    cout << "Gravity is now up" << endl;
    break;

  case 'a':
    solver->acceleration = {-9.8f, 0.0f, 0.0f};
    cout << "Gravity is now left" << endl;
    break;

  case 's':
    solver->acceleration = {0.0f, -9.8f, 0.0f};
    cout << "Gravity is now down" << endl;
    break;

  case 'd':
    solver->acceleration = {0.0f, 9.8f, 0.0f};
    cout << "Gravity is now right" << endl;
    break;

  case 'z':
    solver->acceleration = {0.0f, 0.0f, 9.8f};
    cout << "Gravity is now forward" << endl;
    break;

  case 'x':
    solver->acceleration = {0.0f, 0.0f, -9.8f};
    cout << "Gravity is now back" << endl;
    break;

    case 'q': case 'Q':	// q or esc - quit
  case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}

int main(int argc, char *argv[]) {

  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Camera Test");

  // initialize the camera and such
  initCamera();
  initSimulation();

  // set up opengl callback functions
  glutDisplayFunc(perspDisplay);
  glutIdleFunc(simulateBall);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);


  glutMainLoop();
  return(0);
}

