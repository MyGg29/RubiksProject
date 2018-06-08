
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "header.h"

#define PI 3.14159265


int faceColor[6][3] = { 
    {0,255,0},
    {255,0,0},
    {0,0,255},
    {255,255,0},
    {255,0,255},
    {0,255,255}};
double camPosR = 9, camPosTheta = 0, camPosPhi = 0;
Rubiks mrubiks;

Face initFace(){
    Face face;
    face.color[0] = 255;
    face.color[1] = 255;
    face.color[2] = 255;
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

    //bottomFace already initialized as yellow
    bottomFace.cColor = 'b';
    //vert
    frontFace.color[0] = 0;
    frontFace.color[1] = 255;
    frontFace.color[2] = 0;
    frontFace.cColor = 'g';
    //blanc
    upFace.color[0] = 255;
    upFace.color[1] = 255;
    upFace.color[2] = 0;
    upFace.cColor = 'y';
    //rouge
    backFace.color[0] = 255;
    backFace.color[1] = 0;
    backFace.color[2] = 0;
    backFace.cColor = 'r';
    //blue
    leftFace.color[0] = 0;
    leftFace.color[1] = 0;
    leftFace.color[2] = 255;
    leftFace.cColor = 'b';
    //orange
    rightFace.color[0] = 255;
    rightFace.color[1] = 153;
    rightFace.color[2] = 51;
    rightFace.cColor = 'o';


    
    Cube* cube = malloc(sizeof(Cube));
    cube->face[0] = upFace; 
    cube->face[1] = rightFace;
    cube->face[2] = bottomFace;
    cube->face[3] = leftFace;
    cube->face[4] = frontFace;
    cube->face[5] = backFace;
    
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

void API(Rubiks rubiks)
{

    char inputFormat[20];
    //find edges
    char color = rubiks.cube[1][0][0].face[FRONT].cColor;
    char color2 = rubiks.cube[1][0][0].face[UP].cColor;
    printf("%c, %c", color, color2);

	for(int i=0; i<20; i++)
	{
		printf("%s ",rubiks.edgesNCornersPos[i]);
	}
    printf(";");
    fflush(stdout);
    
}

Rubiks drawRubik() {
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
    



    //bellow,pos [] makes a writtable string, important for strtok;
	char pos[] ="UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR";
	char * pch;
	int i = 0;
	pch = strtok (pos," ,.-");
	while (pch != NULL)
	{
        char* value = malloc(strlen(pch)*sizeof(char));//allocate only what's needed
        strcpy(value, pch);
        rubiks->edgesNCornersPos[i] = value;
		pch = strtok (NULL, " ,.-");
		i++;
	}
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
                //if(j!=0)
                showCube(rubiks.cube[i][j][k]);
            }
        }
    }
}

void display(void)
{
    //TODO DRAW = INIT ???
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showRubiks(mrubiks);
    updateLookAt();
    drawAxis();
    API(mrubiks);
    glutSwapBuffers();
}

void
init(void)
{
   mrubiks = drawRubik();
   printf("out\n");
	for(int i=0; i<20; i++)
	{
		printf("%s ",mrubiks.edgesNCornersPos[i]);
	}
    fflush(stdout);
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
    double delta = 0.05;
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
  glutInitWindowSize(500,500);
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
