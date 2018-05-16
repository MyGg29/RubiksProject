
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


typedef struct _Face {
    GLfloat corner[4][3];
    GLfloat color[3];
} Face;

typedef struct _Cube {
    struct _Face face [6];
} Cube;

typedef struct _RubiksFace {
    struct _Cube cube [9];
} RubiksFace;

typedef struct _Rubiks {
    struct _RubiksFace face [6];
} Rubiks;

void updateLookAt();
void drawCube();
void showFace(Face face);

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat coords[4][3] = {
    {-1, 1, 1},
    {1, 1, 1},
    {1, 1, -1},
    {-1, 1, -1}};
 


GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
int faceColor[6][3] = { 
    {0.0,1.0,0.0},
    {1.0,0.0,0.0},
    {0.0,0.0,1.0},
    {1.0,1.0,0.0},
    {1.0,0.0,1.0},
    {0.0,1.0,1.0}};
double camPosR = 9, camPosTheta = 0, camPosPhi = 0;

Face initFace(){
    Face face;
    
    face.color[0] = 1;
    face.color[1] = 1;
    face.color[2] = 0;
    face.corner[0][0] = 1; //top left 
    face.corner[0][1] = 1; 
    face.corner[0][2] = -1; 
    face.corner[1][0] = -1; //top right
    face.corner[1][1] = 1; 
    face.corner[1][2] = -1; 
    face.corner[2][0] = -1; //bottom right
    face.corner[2][1] = 1; 
    face.corner[2][2] = 1; 
    face.corner[3][0] = 1; //bottom left
    face.corner[3][1] = 1; 
    face.corner[3][2] = 1; 

    return face;
}

void drawAxis() {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3i(0,0,4);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
    glRasterPos3i(0,4,0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
    glRasterPos3i(4,0,0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    glRasterPos3i(0,0,0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	
}

void 
drawCube(GLfloat v[8][3]){
    glRasterPos3i(4,0,0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    int i;
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
}

GLfloat* rotateCornerX(GLfloat corner[3]){
	float rotationMatrix [3][3] = { {1, 0, 0},
									{0, cos(PI/2), -sin(PI/2)},
									{0, sin(PI/2), cos(PI/2)}
								  };
	GLfloat result[3] = {0., 0., 0.};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i] += rotationMatrix[i][j] * corner[j];
		}
	}
	corner = result;
	return corner;
}

void
drawCube2(){
    Face bottomFace = initFace();
	Face frontFace = initFace();
	Face backFace = initFace();
	
	frontFace.corner[2][1] -= 2;
	frontFace.corner[3][1] -= 2;
	frontFace.corner[0][2] += 2;
	frontFace.corner[1][2] += 2;
	
	
	memcpy(frontFace.corner[0], rotateCornerX(frontFace.corner[0]), sizeof frontFace.corner[0]);

	showFace(bottomFace);
	showFace(frontFace);
}

void showFace(Face face) {
    glBegin(GL_QUADS);
    glColor3f(face.color[0], face.color[1], face.color[2]);
    //glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(face.corner[0][0], face.corner[0][1], face.corner[0][2]);
	glVertex3f(face.corner[1][0], face.corner[1][1], face.corner[1][2]);
	glVertex3f(face.corner[2][0], face.corner[2][1], face.corner[2][2]);
	glVertex3f(face.corner[3][0], face.corner[3][1], face.corner[3][2]);

	glEnd();
}

void
drawRubik(void)
{
    int j, k, l;
    glTranslatef(-3, -3, 0);

    for(l = 0; l < 3; l++) {
        glPushMatrix();
        for(k = 0; k < 3; k++) { 
            glPushMatrix();
            for(j = 0; j < 3; j ++) {
                drawCube(v);
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
drawRubik2(void) {
    int j, k, l;
    GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
    /* Setup cube vertex data. */
    drawCube2(v);
}


void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRubik2();
    updateLookAt();
    drawAxis();
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
    /* Z near */ 1.0, /* Z far */ 1000.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)
          , camPosR * sin(camPosTheta)
          , camPosR * cos(camPosTheta)* sin(camPosPhi),  
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
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
        case GLUT_KEY_F2:
            camPosR -= 1;
		break;
	}
	glutPostRedisplay();
}

void updateLookAt()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)
            ,camPosR * sin(camPosTheta)
            , camPosR * cos(camPosTheta)* sin(camPosPhi),  
            0.0, 0.0, 0.0,      /* center is at (0,0,0) */
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
