
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265


typedef struct _Face {
    GLfloat corner[4][3];
    GLfloat color[3];
} Face;

//c'est mieux que d'avoir un array[6] et de se perdre dans l'ordre des faces;
typedef struct _Cube {
    struct _Face face[6];
} Cube;

typedef struct _RubiksFace {
    struct _Cube cube [9];
} RubiksFace;

typedef struct _Rubiks {
    struct _RubiksFace rubiksFace[6];
} Rubiks;

typedef enum {
    DOWN,
    FRONT,
    UP,
    BACK,
    LEFT,
    RIGHT
}enumPosition;


void updateLookAt();
Cube* drawCube();
void showFace(Face face);
void showCube(Cube cube);

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
int faceColor[6][3] = { 
    {0,255,0},
    {255,0,0},
    {0,0,255},
    {255,255,0},
    {255,0,255},
    {0,255,255}};
double camPosR = 9, camPosTheta = 0, camPosPhi = 0;

Face initFace(){
    Face face;
    //jaune par defaut...why not
    face.color[0] = 255;
    face.color[1] = 255;
    face.color[2] = 0;
    face.corner[0][0] = 1; //top left 
    face.corner[0][1] = 1; 
    face.corner[0][2] = -1; 
    face.corner[1][0] = -1; //top right
    face.corner[1][1] = 1 ; 
    face.corner[1][2] = -1; 
    face.corner[2][0] = -1; //bottom right
    face.corner[2][1] = 1 ; 
    face.corner[2][2] = 1 ; 
    face.corner[3][0] = 1 ; //bottom left
    face.corner[3][1] = 1 ; 
    face.corner[3][2] = 1 ; 

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


GLfloat* rotateCorner(GLfloat corner[3], float radAngle, char axe){
    GLfloat* result = malloc(sizeof(GLfloat)*3); 
	result[0] = result[1] = result[2] = 0.;
    //axe doit etre "x", "y" ou "z"
    //matrice de rotation autour de X
	float rotationMatrixX [3][3] = { {1, 0, 0},
									{0, cos(radAngle), -sin(radAngle)},
									{0, sin(radAngle), cos(radAngle)}
								  };
    //autour de Y
    float rotationMatrixY [3][3] = { {cos(radAngle), 0, sin(radAngle)},
                                     {0, 1, 0},
                                     {-sin(radAngle), 0, cos(radAngle)}
                                  };
    //autour de Z
    float rotationMatrixZ [3][3] = { {cos(radAngle), -sin(radAngle), 0},
                                     {sin(radAngle), cos(radAngle), 0},
                                     {0, 0, 1}
                                   };
    float rotationMatrix[3][3];
    switch (axe){
        case 'x':
           memcpy(rotationMatrix, rotationMatrixX, sizeof(rotationMatrixX));
           break;
        case 'y':
           memcpy(rotationMatrix, rotationMatrixY, sizeof(rotationMatrixY));
           break;
        case 'z':
           memcpy(rotationMatrix, rotationMatrixZ, sizeof(rotationMatrixZ));
           break;
    }

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i] += rotationMatrix[i][j] * corner[j];
		}
	}
	return result;
}

//takes in argument the center of the cube who while be drawned, aka the position
Cube* drawCube(){
    Face bottomFace = initFace();
	Face frontFace = initFace();
	Face upFace = initFace();
	Face backFace = initFace();
	Face leftFace = initFace();
	Face rightFace = initFace();
	
    for(int i = 0; i<4; i++){
	    memcpy(frontFace.corner[i], rotateCorner(frontFace.corner[i], PI/2, 'x'), sizeof frontFace.corner[i]);
    }
    for(int i = 0; i<4; i++){
	    memcpy(upFace.corner[i], rotateCorner(upFace.corner[i], PI, 'x'), sizeof upFace.corner[i]);
    }
    for(int i = 0; i<4; i++){
	    memcpy(backFace.corner[i], rotateCorner(backFace.corner[i], 3*PI/2, 'x'), sizeof backFace.corner[i]);
    }
    for(int i = 0; i<4; i++){
	    memcpy(leftFace.corner[i], rotateCorner(leftFace.corner[i], PI/2, 'z'), sizeof leftFace.corner[i]);
    }
    for(int i = 0; i<4; i++){
	    memcpy(rightFace.corner[i], rotateCorner(rightFace.corner[i], -PI/2, 'z'), sizeof rightFace.corner[i]);
    }

    //jaune
    //bottomFace already initialized as yellow
    //vert
    frontFace.color[0] = 0;
    frontFace.color[1] = 255;
    frontFace.color[2] = 0;
    //blanc
    upFace.color[0] = 255;
    upFace.color[1] = 255;
    upFace.color[2] = 255;
    //rouge
    backFace.color[0] = 255;
    backFace.color[1] = 0;
    backFace.color[2] = 0;
    //blue
    leftFace.color[0] = 0;
    leftFace.color[1] = 0;
    leftFace.color[2] = 255;
    //orange
    rightFace.color[0] = 255;
    rightFace.color[1] = 153;
    rightFace.color[2] = 51;


    
    Cube* cube = malloc(sizeof(Cube));
    cube->face[0] = bottomFace; 
    cube->face[1] = frontFace;
    cube->face[2] = upFace;
    cube->face[3] = backFace;
    cube->face[4] = leftFace;
    cube->face[5] = rightFace;
    
    return cube;
}

void showCube(Cube cube){
    for(int i=0; i<6; i++){
        showFace(cube.face[i]);
    }
}

void showFace(Face face) {
    glBegin(GL_QUADS);
    glColor3ub(face.color[0], face.color[1], face.color[2]);
    //glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(face.corner[0][0], face.corner[0][1], face.corner[0][2]);
	glVertex3f(face.corner[1][0], face.corner[1][1], face.corner[1][2]);
    //Commented for now but this line adds gradients to the color
    //glColor3f(face.color[0] - 0.2, face.color[1] - 0.2, face.color[2]);
	glVertex3f(face.corner[2][0], face.corner[2][1], face.corner[2][2]);
	glVertex3f(face.corner[3][0], face.corner[3][1], face.corner[3][2]);
	glEnd();
}

//le problème avec glTranslate et openGl en général, c'est qu'on au aucuns accès aux résultat des transformations que l'on fait, on est donc obligé de prédire le changement ou de le faire à la main.
Cube* translateCube(Cube* cube, GLfloat x, GLfloat y, GLfloat z) {
    for(int i=0; i<6; i++){
        for(int j=0; j<4; j++){
            cube->face[i].corner[j][0] += x;
            cube->face[i].corner[j][1] += y;
            cube->face[i].corner[j][2] += z;
        }
    }
    return cube;
}

void 
drawRubik(void) {
    int center[3] = {0,0,0};
    Cube* cube[27];
    Rubiks* rubiks = malloc(sizeof(Rubiks)); 
    RubiksFace rubiksFace[9];
    for(int i=0; i<27; i++){
        cube[i] = drawCube();
    }

    //cube[0] est déjà en place, on va tourner autour
    //Face
    translateCube(cube[0], -2.1, 0, 0);
    translateCube(cube[1], 0, 0, 0);
    translateCube(cube[2], 2.1, 0, 0);

    translateCube(cube[3], -2.1, -2.1, 0);
    translateCube(cube[4], 0, -2.1, 0);
    translateCube(cube[5], 2.1, -2.1, 0);

    translateCube(cube[6], -2.1, 2.1, 0);
    translateCube(cube[7], 0, 2.1, 0);
    translateCube(cube[8], 2.1, 2.1, 0);
    //Cross
    translateCube(cube[9], 0, 0, 2.1);
    translateCube(cube[10], 0, 0, -2.1);
    translateCube(cube[11], 0, 2.1, 2.1);
    translateCube(cube[12], 0, -2.1, 2.1);
    translateCube(cube[13], 0, 2.1, -2.1);
    translateCube(cube[14], 0, -2.1, -2.1);
    translateCube(cube[15], 2.1, 0, 2.1);
    translateCube(cube[16], -2.1, 0, 2.1);
    translateCube(cube[17], 2.1, 0, -2.1);
    translateCube(cube[18], -2.1, 0, -2.1);
    //corners
    translateCube(cube[19], 2.1, 2.1, 2.1);
    translateCube(cube[20], 2.1, -2.1, 2.1);
    translateCube(cube[21], -2.1, 2.1, -2.1);
    translateCube(cube[22], -2.1, -2.1, -2.1);
    translateCube(cube[23], 2.1, 2.1, -2.1);
    translateCube(cube[24], -2.1, 2.1, 2.1);
    translateCube(cube[25], 2.1, -2.1, -2.1);
    translateCube(cube[26], -2.1, -2.1, 2.1);

    //DOWN, every face translated on Y negativly
    rubiksFace[0].cube[0] = cube[3]; 
    rubiksFace[0].cube[1] = cube[4]; 
    rubiksFace[0].cube[2] = cube[5]; 
    rubiksFace[0].cube[3] = cube[12]; 
    rubiksFace[0].cube[4] = cube[14]; 
    rubiksFace[0].cube[5] = cube[20]; 
    rubiksFace[0].cube[6] = cube[22]; 
    rubiksFace[0].cube[7] = cube[25]; 
    rubiksFace[0].cube[8] = cube[26]; 
    //FRONT, every face translated on X positivly
    rubiksFace[1].cube[0] = cube[2]; 
    rubiksFace[1].cube[1] = cube[5]; 
    rubiksFace[1].cube[2] = cube[8]; 
    rubiksFace[1].cube[3] = cube[15]; 
    rubiksFace[1].cube[4] = cube[17]; 
    rubiksFace[1].cube[5] = cube[19]; 
    rubiksFace[1].cube[6] = cube[20]; 
    rubiksFace[1].cube[7] = cube[23]; 
    rubiksFace[1].cube[8] = cube[25]; 
    //UP, every face translated on Y positivly
    rubiksFace[2].cube[0] = cube[6]; 
    rubiksFace[2].cube[1] = cube[7]; 
    rubiksFace[2].cube[2] = cube[8]; 
    rubiksFace[2].cube[3] = cube[11]; 
    rubiksFace[2].cube[4] = cube[13]; 
    rubiksFace[2].cube[5] = cube[19]; 
    rubiksFace[2].cube[6] = cube[21]; 
    rubiksFace[2].cube[7] = cube[23]; 
    rubiksFace[2].cube[8] = cube[24]; 
    //BACK, every face translated on X negativly
    rubiksFace[3].cube[0] = cube[0]; 
    rubiksFace[3].cube[1] = cube[3]; 
    rubiksFace[3].cube[2] = cube[6]; 
    rubiksFace[3].cube[3] = cube[16]; 
    rubiksFace[3].cube[4] = cube[18]; 
    rubiksFace[3].cube[5] = cube[21]; 
    rubiksFace[3].cube[6] = cube[22]; 
    rubiksFace[3].cube[7] = cube[24]; 
    rubiksFace[3].cube[8] = cube[26]; 
    //LEFT, every face translated on Z negativly
    rubiksFace[4].cube[0] = cube[0]; 
    rubiksFace[4].cube[1] = cube[3]; 
    rubiksFace[4].cube[2] = cube[6]; 
    rubiksFace[4].cube[3] = cube[16]; 
    rubiksFace[4].cube[4] = cube[18]; 
    rubiksFace[4].cube[5] = cube[21]; 
    rubiksFace[4].cube[6] = cube[22]; 
    rubiksFace[4].cube[7] = cube[24]; 
    rubiksFace[4].cube[8] = cube[26]; 

    for(int i=0; i<27; i++){
        showCube(*cube[i]);
    }


}


void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRubik();
    updateLookAt();
    drawAxis();
    glutSwapBuffers();
}

void
init(void)
{
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
  printf("Version de OpenGL %s", glGetString(GL_VERSION));
  fflush(stdout);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
