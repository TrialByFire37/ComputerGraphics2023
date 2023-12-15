#include <GL/glut.h>
#include "SOIL2/SOIL2.h"

GLfloat lightX = 2.0f;
GLfloat lightY = 2.0f;
GLfloat lightZ = 2.0f;

char textures[2][20] = {"wood.png", "city2_5.png"};
GLuint textures_ids[sizeof(textures)];

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

  GLfloat position[] = { lightX, lightY, lightZ, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void specialFunc(int key, int x, int y)
{
  if (key == GLUT_KEY_LEFT)
  {
    lightX -= 0.1f;
  }
  if (key == GLUT_KEY_RIGHT)
  {
    lightX += 0.1f;
  }
  if (key == GLUT_KEY_UP)
  {
    lightY += 0.1f;
  }
  if (key == GLUT_KEY_DOWN)
  {
    lightY -= 0.1f;
  }
}

void keyboardFunc(unsigned char key, int x, int y)
{
  if (key == 27) // esc
  {
    exit(0);
  }
  if (key == 87 || key == 119) // W
  {
    lightZ -= 0.1f;
  }
  if (key == 83 || key == 115) // S
  {
    lightZ += 0.1f;
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

void render_scene()
{
  // планка
  glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures_ids[0]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-4.0f, 0.0f, -4.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-4.0f, 0.0f, 4.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(4.0f, 0.0f, 4.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(4.0f, 0.0f, -4.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // сфера
  glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures_ids[1]);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    GLUquadricObj* sphere;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 0.75, 8, 6);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // конус
  GLfloat diffuse[] = { 0.2, 1.0, 0.8, 1.0 };
  GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat shininess[] = { 128.0f };
  glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glColor3f(0., 1., 0.);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    GLUquadricObj* cone;
    cone = gluNewQuadric();
    gluQuadricDrawStyle(cone, GLU_FILL);
    gluQuadricNormals(cone, GLU_SMOOTH);
    gluQuadricTexture(cone, TRUE);
    gluCylinder(cone, 0.7, 0.0, 2.0, 32, 8);
    glDisable(GL_DEPTH_TEST);
  glPopMatrix();
  
  // цилиндр
  glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.f, 0.f, 1.f, 0.6f);
    glTranslatef(2.0f, -0.5f, 0.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    GLUquadricObj* cylinder;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    gluQuadricTexture(cylinder, TRUE);
    gluCylinder(cylinder, 0.25f, 0.25f, 2.0f, 8, 1);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
  glPopMatrix();

  glColor3f(1.0f, 1.0f, 1.0f);
}

void display(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  init_light();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 800.0 / 600.0, 0.1, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 2.0f, 4.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

  render_scene();

  glutSwapBuffers();
  glutPostRedisplay();
}
