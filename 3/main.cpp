#include <iostream>
#include <GL/glut.h>
#include "tools.hpp"

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(240, 212);
  glutCreateWindow("Lab. 3");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(specialFunc);
  glutReshapeFunc(reshape);

  if (!load_textures())
  {
    std::cout << "Error loading textures.";
    exit(0);
  }

  glutMainLoop();

  return 0;
}
