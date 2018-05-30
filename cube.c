
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265


typedef struct _Face {
    GLfloat corner[4][3];
    GLfloat color[3];
} Face;

typedef struct _Cube {
    struct _Face face[6];
} Cube;

typedef struct _Rubiks {
    struct _Cube cube[3][3][3];
} Rubiks;

typedef struct _FaceAPI {
    int face;//up,down,front,etc
    int color[9][3];//Une couleur par carré présent sur une face
}FaceAPI;

typedef struct _RubiksAPI {
    struct _FaceAPI face[6];//6 faces ds un cube
} RubiksAPI;

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

void initAPI()
{
    RubiksAPI rubiksAPI;
    FaceAPI faceAPI[6];
    int faceId [6][9] = {
        {3,4,5,12,14,20,22,25,26}, //Down
        {2,5,8,15,17,19,20,23,25}, //Front
        {6,7,8,11,13,19,21,23,24}, //UP
        {0,3,6,16,18,21,22,24,26}, //back
        {0,3,6,16,18,21,22,24,26}, //left
        {9,11,12,15,16,19,20,24,26}//right
        };
   /* 
    printf("\n");
    for(int i=0; i<6; i++)
    {
        printf("face %d\n", i);
        for(int j=0; j<9; j++)
        {
            faceAPI[i].face = i;
            faceAPI[i].color[j][0] = (*cube[faceId[i][j]]).face[i].color[0];    
            faceAPI[i].color[j][1] = (*cube[faceId[i][j]]).face[i].color[1];    
            faceAPI[i].color[j][2] = (*cube[faceId[i][j]]).face[i].color[2];    
            printf("%d,%d,%d|", faceAPI[i].color[j][0],faceAPI[i].color[j][1],faceAPI[i].color[j][2]);
        }
        printf("\n||");
    }
    printf("\nend");
    fflush(stdout);
    */
}

Rubiks 
drawRubik() {
    int center[3] = {0,0,0};
    Cube* cube[27];
    Rubiks* rubiks = malloc(sizeof(Rubiks));

    for(int i=0; i<27; i++){
        cube[i] = drawCube();//Init les cubes;
    }
   
    //les places
    //Face
    translateCube(cube[0], -2.1, 0, 0);
    rubiks->cube[2][1][1] = *cube[0];
    translateCube(cube[1], 0, 0, 0);
    rubiks->cube[1][1][1] = *cube[1];
    translateCube(cube[2], 2.1, 0, 0);
    rubiks->cube[0][1][1] = *cube[2];

    translateCube(cube[3], -2.1, -2.1, 0);
    rubiks->cube[2][2][1] = *cube[3];
    translateCube(cube[4], 0, -2.1, 0);
    rubiks->cube[1][2][1] = *cube[4];
    translateCube(cube[5], 2.1, -2.1, 0);
    rubiks->cube[0][2][1] = *cube[5];

    translateCube(cube[6], -2.1, 2.1, 0);
    rubiks->cube[2][0][1] = *cube[6];
    translateCube(cube[7], 0, 2.1, 0);
    rubiks->cube[1][0][1] = *cube[7];
    translateCube(cube[8], 2.1, 2.1, 0);
    rubiks->cube[0][0][1] = *cube[8];
    //Cross
    translateCube(cube[9], 0, 0, 2.1);
    rubiks->cube[1][1][0] = *cube[9];
    translateCube(cube[10], 0, 0, -2.1);
    rubiks->cube[1][1][2] = *cube[10];
    translateCube(cube[11], 0, 2.1, 2.1);
    rubiks->cube[1][0][0] = *cube[11];
    translateCube(cube[12], 0, -2.1, 2.1);
    rubiks->cube[1][2][0] = *cube[12];
    translateCube(cube[13], 0, 2.1, -2.1);
    rubiks->cube[1][0][2] = *cube[13];
    translateCube(cube[14], 0, -2.1, -2.1);
    rubiks->cube[1][2][2] = *cube[14];
    translateCube(cube[15], 2.1, 0, 2.1);
    rubiks->cube[0][1][0] = *cube[15];
    translateCube(cube[16], -2.1, 0, 2.1);
    rubiks->cube[2][1][0] = *cube[16];
    translateCube(cube[17], 2.1, 0, -2.1);
    rubiks->cube[0][1][2] = *cube[17];
    translateCube(cube[18], -2.1, 0, -2.1);
    rubiks->cube[2][1][2] = *cube[18];
    //corners
    translateCube(cube[19], 2.1, 2.1, 2.1);
    rubiks->cube[0][0][0] = *cube[19];
    translateCube(cube[20], 2.1, -2.1, 2.1);
    rubiks->cube[0][2][0] = *cube[20];
    translateCube(cube[21], -2.1, 2.1, -2.1);
    rubiks->cube[2][0][2] = *cube[21];
    translateCube(cube[22], -2.1, -2.1, -2.1);
    rubiks->cube[2][2][2] = *cube[22];
    translateCube(cube[23], 2.1, 2.1, -2.1);
    rubiks->cube[0][0][2] = *cube[23];
    translateCube(cube[24], -2.1, 2.1, 2.1);
    rubiks->cube[2][0][0] = *cube[24];
    translateCube(cube[25], 2.1, -2.1, -2.1);
    rubiks->cube[0][2][2] = *cube[25];
    translateCube(cube[26], -2.1, -2.1, 2.1);
    rubiks->cube[2][2][0] = *cube[26];
    
    
    return *rubiks;

}
void showRubiks(Rubiks rubiks)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<3; k++)
            {
                showCube(rubiks.cube[i][j][k]);
            }
        }
    }
}

Cube rotateCube(Cube cube, float angle ,char axe)
{
	float trans = 0.0;
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<4; j++){
			if(axe == 'x')
			{
				memcpy(cube.face[i].corner[j],rotateCorner(cube.face[i].corner[j], angle, 'x'), sizeof(cube.face[i].corner[j]));

			}
			if(axe == 'y')
			{
				memcpy(cube.face[i].corner[j],rotateCorner(cube.face[i].corner[j], angle, 'y'), sizeof(cube.face[i].corner[j]));

			}
			if(axe == 'z')
			{
				memcpy(cube.face[i].corner[j],rotateCorner(cube.face[i].corner[j], angle, 'z'), sizeof(cube.face[i].corner[j]));

			}
		}
	}
    return cube;
}

void rotateFacePatern(Cube movedCubes[3][3], Cube result[3][3])
{
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            result[i][j] = movedCubes[j][3 - i - 1];
        }
    }
    
}

Rubiks rotateFace(Rubiks rubiks, char face)
{
    //pour chaque cube d'une face ....
    Cube movedCubes[3][3];
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(face==FRONT)
            {
                rubiks.cube[0][i][j] = rotateCube(rubiks.cube[0][i][j], PI/2, 'x');
                movedCubes[i][j] = rubiks.cube[0][i][j];
            }
            if(face==BACK)
            {
                rubiks.cube[2][i][j] = rotateCube(rubiks.cube[2][i][j], PI/2, 'x');
                movedCubes[i][j] = rubiks.cube[2][i][j];
            }
            if(face==DOWN)
            {
                rubiks.cube[i][2][j] = rotateCube(rubiks.cube[i][2][j], PI/2, 'y');
                movedCubes[i][j] = rubiks.cube[i][2][j];
            }
            if(face==UP)
            {
                rubiks.cube[i][0][j] = rotateCube(rubiks.cube[i][0][j], PI/2, 'y');
                movedCubes[i][j] = rubiks.cube[i][0][j];
            }
            if(face==LEFT)
            {
                rubiks.cube[i][j][0] = rotateCube(rubiks.cube[i][j][0], PI/2, 'z');
                movedCubes[i][j] = rubiks.cube[i][j][0];
            }
            if(face==RIGHT)
            {
                rubiks.cube[i][j][2] = rotateCube(rubiks.cube[i][j][2], PI/2, 'z');
                movedCubes[i][j] = rubiks.cube[i][j][2];
            }
        }
    }
    int test[3][3] = { {1,2,3},{4,5,6},{7,8,9}};
    Cube result[3][3];
    
    rotateFacePatern(movedCubes, result);
    
    if(face==FRONT)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[0][i][j] = result[i][j];
    }
    if(face==BACK)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[2][i][j] = result[i][j];
    }
    if(face==DOWN)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[i][2][j] = result[i][j];
    }
    if(face==UP)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[i][0][j] = result[i][j];
    }
    if(face==LEFT)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[i][j][0] = result[i][j];
    }
    if(face==RIGHT)
    {
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            rubiks.cube[i][j][2] = result[i][j];
    }
    for(int i=0; i<3;i++)
        for(int j=0; j<3;j++)
            //rubiks.cube[0][i][j] = result[i][j];

    return rubiks;
}

Rubiks mrubiks;

void display(void)
{
    //TODO DRAW = INIT ???
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showRubiks(mrubiks);
    updateLookAt();
    drawAxis();
    glutSwapBuffers();
}

void
init(void)
{
   mrubiks = drawRubik();
  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective(90.0, /* field of view in degree */ 
                1.0,/* aspect ratio */ 
                1.0, /* Z near */
                1000.0); /* Z far */
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)
          , camPosR * sin(camPosTheta)
          , camPosR * cos(camPosTheta)* sin(camPosPhi),  
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

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
void key (unsigned char key, int xmouse, int ymouse)
{
    switch(key){
        case 'f':
            mrubiks = rotateFace(mrubiks, FRONT);
            glutPostRedisplay();
            break;
        case 'b':
            mrubiks = rotateFace(mrubiks, BACK);
            glutPostRedisplay();
            break;
        case 'd':
            mrubiks = rotateFace(mrubiks, DOWN);
            glutPostRedisplay();
            break;
        case 'u':
            mrubiks = rotateFace(mrubiks, UP);
            glutPostRedisplay();
            break;
        case 'l':
            mrubiks = rotateFace(mrubiks, LEFT);
            glutPostRedisplay();
            break;
        case 'r':
            mrubiks = rotateFace(mrubiks, RIGHT);
            glutPostRedisplay();
            break;
    }
    fflush(stdout);
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
  glutKeyboardFunc(key);
  init();
  printf("Version de OpenGL %s", glGetString(GL_VERSION));
  fflush(stdout);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
