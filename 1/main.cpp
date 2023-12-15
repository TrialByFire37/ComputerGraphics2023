#include <iostream>
#include <GL/glut.h>
#include "tools.hpp"

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(240, 212);
  glutCreateWindow("Lab. 1");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboardFunc);
  glutMainLoop();

  return 0;
}
