# Solver for 6-plane Collision With A Ball

This project shows a eulerian based solver for ball collision
with six sides. There are many parameters that can be tweaked for to
get a different behavior out of the ball.

## Assignment
Physically Based Animation (CPSC 8170) assignment 1.

For this assignment, the object is to get everyone on a firm footing with the basic notions of physical simulation, and the use of OpenGL and GLUT as an interactive simulation and visualization environment. Build a simulation of a 3D spherical ball bouncing in a box. The box should have all six sides, but at least some of the sides should be invisible so that it is possible to view the ball. For this assignment, you may want to use my matrix and vector routines to make it easier to write the code involving vector mathematics. Note that my bouncing ball example may not be a very good place to start from, since it is done in 2D, and it is intentionally written to slow down the simulation for instruction in class. For this homework, you need to be working in 3D, with a virtual camera. Also, you will probably want the simulation to run as fast as it can, adjusting the time step h to speed up or slow down the simulation speed and display update.


## Features
* Adjustable box size (width, height, depth)
* Adjustable ball position (x, y, z)
* Adjustable ball radius
* Basic restitution calculation
* Basic friction calculation
* Basic air resistance calculation
* Multi-Wall collision detection

## Un-Implemented
* Basic wind calculation
* Multi-Wall collision response
* Resting condition detection

## Usage
```
$> BouncingBall parameters
```

## Interaction
```
w   - Make gravity point up
a   - Make gravity point left
s   - Make gravity point down
d   - Make gravity point right
z   - Make gravity point forward
x   - Make gravity point back
f   - Reset the camera
g   - Toggle grid
esc - Quit the program
q   - Quit the program
```

## Acknowledgements
Vector Class - Donald House

Matrix Class - Donald House

Camera Class - Christopher Root, Donald House, Yujie Shu

