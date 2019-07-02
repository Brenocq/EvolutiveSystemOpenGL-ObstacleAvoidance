#include <GL/glut.h>
#include <iostream>
#include <sys/time.h>
#include <math.h>

using namespace std;

#define WindowHeight 800
#define WindowWidth 800
#define ScreenHeight 1080
#define ScreenWidth 1920
int factor=100; // factor the animation is slowed down by

double dx = 0;
double size = 1;

void animate(double speed);

static double ftime(void) {
  struct timeval t;
  gettimeofday(&t, NULL);

  return 1.0*t.tv_sec + 1e-6*t.tv_usec;
}

void animate(double speed) {
  if (dx > 1.5) {
    dx = -1.5;
  }
  else {
    dx += speed/factor;
    size = (cos((dx+1.5)*M_PI*3)+1)/4;
    cout<< (dx+1.5)/3*M_PI <<" "<<cos((dx+1.5)/3*M_PI)<<endl;
  }
  glutPostRedisplay();
}

void square(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 0.5);

  glBegin(GL_POLYGON);
    glVertex2d(-(size/2)+dx, (size/2)+dx);
    glVertex2d(-(size/2)+dx, -(size/2)+dx);
    glVertex2d((size/2)+dx, -(size/2)+dx);
    glVertex2d((size/2)+dx, (size/2)+dx);
  glEnd();

  glutSwapBuffers();
}


static double last_T;

static void idle(void) {
  const double now_T = ftime();
  const double delta_T = now_T - last_T;
  last_T = now_T;

  const double speed = delta_T * 60;

  animate(speed);
  glutPostRedisplay();
}

void initialize(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitWindowPosition((ScreenWidth/2)-(WindowWidth/2), (ScreenHeight/2)-(WindowHeight/2));
    glutCreateWindow("Hello world!");

    initialize();
    glutDisplayFunc(square);
    glutIdleFunc(idle);
    glutPostRedisplay();

    glutMainLoop();
    return 0;
}
