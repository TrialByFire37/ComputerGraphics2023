#include <GL/glut.h>

int task_flag = 1;
bool anim_flag = false;

GLfloat sphere_x = 2.0f;
GLfloat cone_x = -1.0f;
GLfloat cone_y = -1.0f;
GLfloat cylinder_angle = -90.0f;
GLfloat torus_scale = 1.0f;

void values_reinit()
{
  sphere_x = 2.0f;
  cone_x = -1.0f;
  cone_y = -1.0f;
  cylinder_angle = -90.0f;
  torus_scale = 1.0f;
}

void timer(int)
{
  if (anim_flag == true)
  {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);

    if (task_flag == 1) {
      if (sphere_x > 0.0f)
      {
        sphere_x -= 0.2f;
      }

      if (cone_x < 0.0f)
      {
        cone_x += 0.1f;
        cone_y += 0.1f;
      }
    }
    else
    {
      if (cylinder_angle < 135.0f)
      {
        cylinder_angle += 5.0f;
      }

      if (torus_scale < 2.0f && cylinder_angle == 135.0f)
      {
        torus_scale += 0.1f;
      }
    }
  }
}

void keyboardFunc(unsigned char key, int x, int y)
{
  if (key == 27) // esc
  {
    exit(0);
  }
  if (key == 32) // space
  {
    anim_flag = true;
    glutTimerFunc(0, timer, 0);
  }
  if (key == 49) // 1
  {
    task_flag = 1;
    anim_flag = false;
    values_reinit();
  }
  if (key == 50) // 2
  {
    task_flag = 2;
    anim_flag = false;
    values_reinit();
  }
}

void display(void)
{
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.f, 640.f / 480.f, 1.f, 100.f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.f, 0.f, 5.f,
    0.f, 0.f, 0.f,
    0.f, 1.f, 0.f);

  if (task_flag == 1)
  {
    glColor3f(1., 0., 0.);
    glPushMatrix();
    glTranslatef(sphere_x, 0.0f, 0.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glutWireSphere(0.75, 8.0f, 6.0f);
    glPopMatrix();

    glColor3f(0., 0., 1.);
    glPushMatrix();
    glTranslatef(cone_x, cone_y, 0.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glutWireCone(0.7, 2.0, 7.0, 6.0);
    glPopMatrix();
  }
  else
  {
    glColor3f(0., 0., 1.);
    glPushMatrix();
    glTranslatef(-1.0f, -1.0f, 0.0f);
    glRotatef(cylinder_angle, 1.0f, 0.0f, 0.0f);
    GLUquadricObj* quadric;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_LINE);
    gluCylinder(quadric, 0.25f, 0.25f, 2.0f, 8, 8);
    glPopMatrix();

    glColor3f(1., 0., 0.);
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glScalef(torus_scale, torus_scale, torus_scale);
    glutWireTorus(0.25, 0.75, 24.0f, 16.0f);
    glPopMatrix();
  }
  
  glutSwapBuffers();
  glutPostRedisplay();
}
