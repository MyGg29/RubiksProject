
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265


void updateLookAt();

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
int faceColor[6][3] = { 
    {0.0,1.0,0.0},
    {1.0,0.0,0.0},
    {0.0,0.0,1.0},
    {1.0,1.0,0.0},
    {1.0,0.0,1.0},
    {0.0,1.0,1.0}};
double camPosR = 9, camPosTheta = 0, camPosPhi = 0;

void
drawBox(void)
{
    int i, j, k, l;


    for(l = 0; l < 3; l++) {
        glPushMatrix();
        for(k = 0; k < 3; k++) { 
            glPushMatrix();
            for(j = 0; j < 3; j ++) {
                for (i = 0; i < 6; i++) {
                    glBegin(GL_QUADS);
                    glColor3f(faceColor[i][0],faceColor[i][1],faceColor[i][2]);
                    glNormal3fv(&n[i][0]);
                    glVertex3fv(&v[faces[i][0]][0]);
                    glVertex3fv(&v[faces[i][1]][0]);
                    glVertex3fv(&v[faces[i][2]][0]);
                    glVertex3fv(&v[faces[i][3]][0]);
                    glEnd();
                }
                glTranslatef(2.1, 0.0, 0.0);
            }
            glPopMatrix();
            glTranslatef(0.0, 2.1, 0.0);
        }
        glPopMatrix();
        glTranslatef(0.0, 0.0, 2.1);
    }
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  updateLookAt();
  glutSwapBuffers();
}

void
init(void)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 90.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)
          , camPosR * sin(camPosTheta)
          , camPosR * cos(camPosTheta)* sin(camPosPhi),  
    0.0, 3.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  //glTranslatef(0.0, 0.0, -1.0);
  //glRotatef(60, 1.0, 0.0, 0.0);
  //glRotatef(-20, 0.0, 0.0, 1.0);
}

void 
arrows (int key, int x, int y) 
{
    double delta = 0.1;
  	switch(key)
	{
		case GLUT_KEY_UP:
            camPosTheta += delta;
			break;
		case GLUT_KEY_DOWN:
            camPosTheta -= delta;
			break;
		case GLUT_KEY_LEFT:
            camPosPhi -= delta;
			break;
		case GLUT_KEY_RIGHT:
            camPosPhi += delta;
        case GLUT_KEY_F1:
            camPosR += 1;
		break;
	}

	glutPostRedisplay();
}

void updateLookAt()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)
            ,3 + camPosR * sin(camPosTheta)
            , camPosR * cos(camPosTheta)* sin(camPosPhi),  
            0.0, 3.0, 0.0,      /* center is at (0,0,0) */
            0.0, 1.0, 0.0);      /* up is in positive Y direction */


}


int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  glutSpecialFunc(arrows);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
