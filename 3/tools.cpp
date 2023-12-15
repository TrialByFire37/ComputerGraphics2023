#define _USE_MATH_DEFINES

#include <chrono>
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "SOIL2/SOIL2.h"

GLfloat icoTransX = 0.0f;
GLfloat icoTransY = -2.2f;
GLfloat icoTransZ = -4.9f;
GLfloat icoAngleX = -79.5f;
GLfloat icoAngleY = 18.0f;
GLfloat icoAngleZ = 26.0f;

GLfloat cameraTransX = 0.0f;
GLfloat cameraTransY = 0.0f;
GLfloat cameraTransZ = 3.0f;
GLfloat cameraAngleX = 0.0f;
GLfloat cameraAngleY = 0.0f;
GLfloat cameraAngleZ = 0.0f;

int move = 0;
int interval = 60;
bool animation = false;

char textures[2][20] = {"city6_3.png", "wmet3_3.png"};
GLuint textures_ids[sizeof(textures)];

void timer(int value);

long long current_milli_time() {
  auto now = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  auto epoch = milliseconds.time_since_epoch();
  return static_cast<long long>(epoch.count());
}

void init_light()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  GLfloat position[] = { 0.0f, 2.0f, 0.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void keyboardFunc(unsigned char key, int x, int y)
{
  // quit
  if (key == 27) // Esc
  {
    exit(0);
  }

  // animation
  if (key == 32) // space
  {
    animation = true;
    glutTimerFunc(interval, timer, 0);
  }

  // camera translation
  if (key == 49) // 1
  {
    cameraTransX += 0.1f;
  }
  if (key == 50) // 2
  {
    cameraTransX -= 0.1f;
  }
  if (key == 51) // 3
  {
    cameraTransY -= 0.1f;
  }
  if (key == 52) // 4
  {
    cameraTransY += 0.1f;
  }
  if (key == 53) // 5
  {
    cameraTransZ -= 0.1f;
  }
  if (key == 54) // 6
  {
    cameraTransZ += 0.1f;
  }

  // camera rotation
  if (key == 81 || key == 113) // Q
  {
    cameraAngleX += 1.0f;
  }
  if (key == 87 || key == 119) // W
  {
    cameraAngleX -= 1.0f;
  }
  if (key == 69 || key == 101) // E
  {
    cameraAngleY -= 1.0f;
  }
  if (key == 82 || key == 114) // R
  {
    cameraAngleY += 1.0f;
  }
  if (key == 84 || key == 116) // T
  {
    cameraAngleZ -= 1.0f;
  }
  if (key == 89 || key == 121) // Y
  {
    cameraAngleZ += 1.0f;
  }

  // icosahedron translation
  if (key == 65 || key == 97) // A
  {
    icoTransX -= 0.05f;
  }
  if (key == 83 || key == 115) // S
  {
    icoTransX += 0.05f;
  }
  if (key == 68 || key == 100) // D
  {
    icoTransY -= 0.05f;
  }
  if (key == 70 || key == 102) // F
  {
    icoTransY += 0.05f;
  }
  if (key == 71 || key == 103) // G
  {
    icoTransZ -= 0.05f;
  }
  if (key == 72 || key == 104) // H
  {
    icoTransZ += 0.05f;
  }

  // icosahedron rotation
  if (key == 90 || key == 122) // Z
  {
    icoAngleX -= 0.5f;
  }
  if (key == 88 || key == 120) // X
  {
    icoAngleX += 0.5f;
  }
  if (key == 67 || key == 99) // C
  {
    icoAngleY -= 0.5f;
  }
  if (key == 86 || key == 118) // V
  {
    icoAngleY += 0.5f;
  }
  if (key == 66 || key == 98) // B
  {
    icoAngleZ -= 0.5f;
  }
  if (key == 78 || key == 110) // N
  {
    icoAngleZ += 0.5f;
  }
}

void specialFunc(int value, int x, int y)
{
  // front
  if (value == GLUT_KEY_UP)
  {
    cameraTransX = 0.0f;
    cameraTransY = 3.0f;
    cameraTransZ = 0.4f;
    cameraAngleX = 0.0f;
    cameraAngleY = 0.0f;
    cameraAngleZ = 0.0f;
  }

  // left
  if (value == GLUT_KEY_LEFT)
  {
    cameraTransX = 3.0f;
    cameraTransY = 3.0f;
    cameraTransZ = 3.4f;
    cameraAngleX = 90.0f;
    cameraAngleY = 0.0f;
    cameraAngleZ = 0.0f;
  }

  // back
  if (value == GLUT_KEY_DOWN)
  {
    cameraTransX = 0.0f;
    cameraTransY = 3.0f;
    cameraTransZ = 6.3f;
    cameraAngleX = -180.0f;
    cameraAngleY = 0.0f;
    cameraAngleZ = 0.0f;
  }

  // right
  if (value == GLUT_KEY_RIGHT)
  {
    cameraTransX = -3.0f;
    cameraTransY = 3.0f;
    cameraTransZ = 3.4f;
    cameraAngleX = -90.0f;
    cameraAngleY = 0.0f;
    cameraAngleZ = 0.0f;
  }

  // top
  if (value == GLUT_KEY_F1)
  {
    cameraTransX = 0.0f;
    cameraTransY = -3.0f;
    cameraTransZ = -0.8f;
    cameraAngleX = 0.0f;
    cameraAngleY = 90.0f;
    cameraAngleZ = 0.0f;
  }

  // last used
  if (value == GLUT_KEY_F2)
  {
    cameraTransX = 0.4f;
    cameraTransY = 2.8f;
    cameraTransZ = -1.1f;
    cameraAngleX = 8.0f;
    cameraAngleY = 0.0f;
    cameraAngleZ = 0.0f;
  }
}

bool load_textures()
{
  for (int i = 0; i < 2; i++)
  {
    textures_ids[i] = SOIL_load_OGL_texture(textures[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    if (textures_ids[i] != 0)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
      return false;
    }
  }
  return true;
}

void render_cube()
{
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textures_ids[0]);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0, -3.0, 0.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(3.0, -3.0, 0.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.0, -9.0, 0.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-3.0, -9.0, 0.0);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0, -3.0, 0.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-3.0, -3.0, -6.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-3.0, -9.0, -6.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-3.0, -9.0, 0.0);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0, -3.0, -6.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(3.0, -3.0, -6.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.0, -9.0, -6.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-3.0, -9.0, -6.0);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(3.0, -3.0, -6.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(3.0, -3.0, 0.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.0, -9.0, 0.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(3.0, -9.0, -6.0);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0, -9.0, 0.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-3.0, -9.0, -6.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.0, -9.0, -6.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(3.0, -9.0, 0.0);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0, -3.0, 0.0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-3.0, -3.0, -6.0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.0, -3.0, -6.0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(3.0, -3.0, 0.0);
  glEnd();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void render_ico()
{
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textures_ids[1]);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glTranslatef(icoTransX, icoTransY, icoTransZ);
  glRotatef(icoAngleX, 1.0f, 0.0f, 0.0f);
  glRotatef(icoAngleY, 0.0f, 1.0f, 0.0f);
  glRotatef(icoAngleZ, 0.0f, 0.0f, 1.0f);
  glutSolidIcosahedron();
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void timer(int value)
{
  if (animation == true)
  {
    if (move < 60)
    {
      icoTransZ += (0.2f / 60);
      icoAngleX += (42.0f / 60);
    }
    if (move >= 60 && move < 120)
    {
      icoTransX -= (0.1f / 60);
      icoTransZ += (0.3f / 60);
      icoAngleX += (60.0f / 60);
      icoAngleY += (4.5f / 60);
      icoAngleZ += (23.0f / 60);
    }
    if (move >= 120 && move < 180)
    {
      icoTransX -= (0.3f / 60);
      icoTransZ += (0.3f / 60);
      icoAngleX += (24.5f / 60);
      icoAngleY += (15.5f / 60);
      icoAngleZ += (21.0f / 60);
    }
    if (move >= 180 && move < 240)
    {
      icoTransX += (0.15f / 60);
      icoTransZ += (0.2f / 60);
      icoAngleX += (42.5f / 60);
      icoAngleY -= (17.0f / 60);
      icoAngleZ += (11.0f / 60);
    }
    if (move >= 240 && move < 300)
    {
      icoTransX += (0.2f / 60);
      icoTransZ += (0.2f / 60);
      icoAngleX += (55.5f / 60);
      icoAngleY -= (26.5f / 60);
      icoAngleZ += (18.0f / 60);
    }
    if (move >= 300 && move < 360)
    {
      icoTransX += (0.25f / 60);
      icoTransZ += (0.15f / 60);
      icoAngleX += (34.0f / 60);
      icoAngleY -= (9.0f / 60);
      icoAngleZ += (27.0f / 60);
    }
    if (move >= 360 && move < 420)
    {
      icoTransX -= (0.05f / 60);
      icoTransZ += (0.15f / 60);
      icoAngleX += (38.0f / 60);
      icoAngleY -= (6.0f / 60);
      icoAngleZ -= (8.0f / 60);
    }
    if (move >= 420 && move < 480)
    {
      icoTransX += (0.15f / 60);
      icoTransZ += (0.15f / 60);
      icoAngleX += (32.5f / 60);
      icoAngleY += (22.5f / 60);
      icoAngleZ += (20.5f / 60);
    }
    if (move >= 480 && move < 540)
    {
      icoTransZ += (0.15f / 60);
      icoAngleX += (40.5f / 60);
      icoAngleY -= (4.5f / 60);
    }
    if (move >= 540)
    {
      animation = false;
    }

    move += 1;
    interval -= 1;
    std::cout << "move: " << move << "\n";
    glutTimerFunc(interval, timer, 0);

    if (interval == 0)
    {
      interval = 60;
    }
  }
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
}

void display(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, (float) 800 / (float) 600, 0.1, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, 2.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

  init_light();

  glTranslatef(cameraTransX, cameraTransY, -cameraTransZ);
  glRotatef(cameraAngleX, 0.0f, 1.0f, 0.0f);
  glRotatef(cameraAngleY, 1.0f, 0.0f, 0.0f);
  glRotatef(cameraAngleZ, 0.0f, 0.0f, 1.0f);

  render_cube();
  render_ico();

  glutSwapBuffers();
  glutPostRedisplay();
}
