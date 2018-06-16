#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
Rubiks msolvedRubiks;
int reverseRotation = 0;

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
    //bleu
    frontFace.color[0] = 0;
    frontFace.color[1] = 0;
    frontFace.color[2] = 255;
    frontFace.cColor = 'g';
    //blanc
    upFace.color[0] = 255;
    upFace.color[1] = 255;
    upFace.color[2] = 0;
    upFace.cColor = 'y';
    //vert
    backFace.color[0] = 0;
    backFace.color[1] = 255;
    backFace.color[2] = 0;
    backFace.cColor = 'r';
    //blue
    leftFace.color[0] = 255;
    leftFace.color[1] = 0;
    leftFace.color[2] = 0;
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


void doTPermutation()
{
    //T permutation
    //R U' R’ U R’ F' R2 U R’ U R U' R’ F 
    //from https://ruwix.com/the-rubiks-cube/how-to-solve-the-rubiks-cube-blindfolded-tutorial/ with changes because my F is the site F'. Same between U and U'
    mrubiks = rotateFace(mrubiks, RIGHT, PI/2);
    mrubiks = rotateFace(mrubiks, UP, -PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, -PI/2);
    mrubiks = rotateFace(mrubiks, UP, PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, -PI/2);
    mrubiks = rotateFace(mrubiks, FRONT, -PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, PI/2);
    mrubiks = rotateFace(mrubiks, UP, PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, -PI/2);
    mrubiks = rotateFace(mrubiks, UP, PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, PI/2);
    mrubiks = rotateFace(mrubiks, UP, -PI/2);
    mrubiks = rotateFace(mrubiks, RIGHT, -PI/2);
    mrubiks = rotateFace(mrubiks, FRONT, PI/2);
}

int* find(char* edgeName, Rubiks rubiks)
{
    int* result = malloc(3*sizeof(int));
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                if(strcmp(edgeName, rubiks.cube[i][j][k].edgesNCornersName)==0)
                {
                    *result = i; 
                    *(result+1) = j; 
                    *(result+2) = k; 
                }
    return result;
}

void API(Rubiks rubiks)
{
    //Y permutation
    //F' R U R’ U R U' R’ F R U' R’ U R’ F' R F 
    //

    char* edgesName[11] = {"FL","FR","DF","UF","UL","DL","DR","UB","BL","BR","BD"};
    //All moves followed by the angle to move them, so so 1 is PI/2, -1 is -PI/2
    int* edgesPreMoves[11] = {
        (int[2]){LEFT,1},
        (int[6]){UP,1,FRONT,1,UP,-1},
        (int[6]){DOWN,-1,LEFT,1,LEFT,1},
        (int[8]){RIGHT,1,FRONT,1,RIGHT,-1,LEFT,1},
        (int[0]){},//no moves requiered to move it to the right pos
        (int[4]){LEFT,1,LEFT,1},
        (int[8]){DOWN,1,DOWN,1,LEFT,1,LEFT,1},//minus are for opposite rotations
        (int[8]){RIGHT,-1,BACK,1,RIGHT,1,LEFT,-1},
        (int[2]){LEFT,-1},
        (int[6]){UP,-1,BACK,1,UP,1},
        (int[6]){DOWN,1,LEFT,1,LEFT,1}
    };
    int premovesSize[11] = {2,6,6,8,0,4,8,8,2,6,6};
    char* edgesDone[20] = {"FL","0","FR","0","DF","0","UF","0","DL","0","DR","0","UB","0","BL","0","BR","0","BD","0"};

    int* rightBufferPos;
    char* inBuffer = rubiks.cube[1][0][2].edgesNCornersName;//what in the buffer
    printf("inBuffer %s", inBuffer);
    fflush(stdout);
    rightBufferPos = find(inBuffer, msolvedRubiks);//Where buffer is supposed to be

    if(*(rightBufferPos) != 1 || *(rightBufferPos+1) != 0 || *(rightBufferPos+2) != 2)//the buffer isn't blocked 
    {
        int index=0;
        printf("segFault ? ");
        fflush(stdout);
        for(index=0;strcmp(edgesName[index],inBuffer)!=0;index++);//We get the index of the buffer in edgesName
       // while(strcmp(edgesName[index],inBuffer)!=0)
       // {
       //    index++;
       // }
        //apply the premoves
        for(int i=0; i<premovesSize[index]; i=i+2)
        {
            float angle = PI/2*edgesPreMoves[index][i+1];
            mrubiks = rotateFace(mrubiks, edgesPreMoves[index][i],angle);
        }

        doTPermutation();

        //Do the opposites of the premoves
        for(int i=premovesSize[index]-2; i>=0; i=i-2)
        {
            float angle = -PI/2*edgesPreMoves[index][i+1];
            mrubiks = rotateFace(mrubiks, edgesPreMoves[index][i],angle);
        }
        //edgesDone[index*2+1] = "1";
            //printf("%s,",edgesDone[19]);
        //    fflush(stdout);
    }
    else//the buffer is blocked
    {
        
    }

        

   /* 
    printf("\n");
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<3; k++)
                //printf("%s\t,", rubiks.cube[i][j][k].edgesNCornersName);
        printf("\n");
        }
    printf("\n");
    }
    */
    int solved = isEqual(rubiks, msolvedRubiks);
    //printf("Solved ? : %d ", solved);

    //printf(";");
    fflush(stdout);
    
}

void resetCube()
{
    mrubiks = drawRubik();
}

int isEqual(Rubiks rubiks, Rubiks rubiks2)
{
    int equal = 1;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                if(rubiks.cube[i][j][k].edgesNCornersName != rubiks2.cube[i][j][k].edgesNCornersName)
                    equal = 0;
    return equal;
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
    rubiks->cube[2][1][1] = *cube[0];//blue center
    rubiks->cube[2][1][1].edgesNCornersName = "B";
    

    translateCube(cube[1], 0, 0, 0);
    rubiks->cube[1][1][1] = *cube[1];//centre (pas visible)
    rubiks->cube[1][1][1].edgesNCornersName = "C";
    
    translateCube(cube[2], 2.1, 0, 0);
    rubiks->cube[0][1][1] = *cube[2];//centre orange
    rubiks->cube[0][1][1].edgesNCornersName = "O";

    translateCube(cube[3], -2.1, -2.1, 0);
    rubiks->cube[2][2][1] = *cube[3];
    rubiks->cube[2][2][1].edgesNCornersName = "DB";

    translateCube(cube[4], 0, -2.1, 0);
    rubiks->cube[1][2][1] = *cube[4];//yellow center
    rubiks->cube[1][2][1].edgesNCornersName = "Y";
    
    translateCube(cube[5], 2.1, -2.1, 0);
    rubiks->cube[0][2][1] = *cube[5];
    rubiks->cube[0][2][1].edgesNCornersName = "DF";


    translateCube(cube[6], -2.1, 2.1, 0);
    rubiks->cube[2][0][1] = *cube[6];
    rubiks->cube[2][0][1].edgesNCornersName = "UB";

    translateCube(cube[7], 0, 2.1, 0);
    rubiks->cube[1][0][1] = *cube[7];//centre blanc
    rubiks->cube[1][0][1].edgesNCornersName = "W";

    translateCube(cube[8], 2.1, 2.1, 0);
    rubiks->cube[0][0][1] = *cube[8];
    rubiks->cube[0][0][1].edgesNCornersName = "UF";
    //Cross
    translateCube(cube[9], 0, 0, 2.1);
    rubiks->cube[1][1][0] = *cube[9];//Green center
    rubiks->cube[1][1][0].edgesNCornersName = "G";
    translateCube(cube[10], 0, 0, -2.1);
    rubiks->cube[1][1][2] = *cube[10];//Red center
    rubiks->cube[1][1][2].edgesNCornersName = "R";
    translateCube(cube[11], 0, 2.1, 2.1);
    rubiks->cube[1][0][0] = *cube[11];
    rubiks->cube[1][0][0].edgesNCornersName = "UL";
    translateCube(cube[12], 0, -2.1, 2.1);
    rubiks->cube[1][2][0] = *cube[12];
    rubiks->cube[1][2][0].edgesNCornersName = "DL";

    translateCube(cube[13], 0, 2.1, -2.1);
    rubiks->cube[1][0][2] = *cube[13];
    rubiks->cube[1][0][2].edgesNCornersName = "UR";

    translateCube(cube[14], 0, -2.1, -2.1);
    rubiks->cube[1][2][2] = *cube[14];
    rubiks->cube[1][2][2].edgesNCornersName = "DR";
    
    translateCube(cube[15], 2.1, 0, 2.1);
    rubiks->cube[0][1][0] = *cube[15];
    rubiks->cube[0][1][0].edgesNCornersName = "FL";

    translateCube(cube[16], -2.1, 0, 2.1);
    rubiks->cube[2][1][0] = *cube[16];
    rubiks->cube[2][1][0].edgesNCornersName = "BL";

    translateCube(cube[17], 2.1, 0, -2.1);
    rubiks->cube[0][1][2] = *cube[17];
    rubiks->cube[0][1][2].edgesNCornersName = "FR";
    translateCube(cube[18], -2.1, 0, -2.1);
    rubiks->cube[2][1][2] = *cube[18];
    rubiks->cube[2][1][2].edgesNCornersName = "BR";
    //corners
    translateCube(cube[19], 2.1, 2.1, 2.1);
    rubiks->cube[0][0][0] = *cube[19];
    rubiks->cube[0][0][0].edgesNCornersName = "ULF";
    translateCube(cube[20], 2.1, -2.1, 2.1);
    rubiks->cube[0][2][0] = *cube[20];
    rubiks->cube[0][2][0].edgesNCornersName = "DFL";
    translateCube(cube[21], -2.1, 2.1, -2.1);
    rubiks->cube[2][0][2] = *cube[21];
    rubiks->cube[2][0][2].edgesNCornersName = "URB";
    translateCube(cube[22], -2.1, -2.1, -2.1);
    rubiks->cube[2][2][2] = *cube[22];
    rubiks->cube[2][2][2].edgesNCornersName = "DBR";
    translateCube(cube[23], 2.1, 2.1, -2.1);
    rubiks->cube[0][0][2] = *cube[23];
    rubiks->cube[0][0][2].edgesNCornersName = "UFR";
    translateCube(cube[24], -2.1, 2.1, 2.1);
    rubiks->cube[2][0][0] = *cube[24];
    rubiks->cube[2][0][0].edgesNCornersName = "UBL";
    translateCube(cube[25], 2.1, -2.1, -2.1);
    rubiks->cube[0][2][2] = *cube[25];
    rubiks->cube[0][2][2].edgesNCornersName = "DRF";
    translateCube(cube[26], -2.1, -2.1, 2.1);
    rubiks->cube[2][2][0] = *cube[26];
    rubiks->cube[2][2][0].edgesNCornersName = "DLB";
    

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
                //if(i!=1 || j!=0 || k != 0)
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
    glutSwapBuffers();
}

void
init(void)
{
   mrubiks = drawRubik();
   msolvedRubiks = drawRubik();
   printf("out\n");
	for(int i=0; i<20; i++)
	{
	}
    fflush(stdout);
  /* Use depth buffering for hidden surface elimination. */
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_SMOOTH);

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
    if(camPosTheta>PI/2)
        camPosTheta=PI/2;
    if(camPosTheta<-PI/2)
        camPosTheta=-PI/2;;
	glutPostRedisplay();
}
void key (unsigned char key, int xmouse, int ymouse)
{
    float angle;
    if(glutGetModifiers() == GLUT_ACTIVE_ALT)
        angle = -PI/2;
    else
        angle = PI/2;
    switch(key){
        case 'f':
            mrubiks = rotateFace(mrubiks, FRONT, angle);
            break;
        case 'b':
            mrubiks = rotateFace(mrubiks, BACK, angle);
            break;
        case 'd':
            mrubiks = rotateFace(mrubiks, DOWN, angle);
            break;
        case 'u':
            mrubiks = rotateFace(mrubiks, UP, angle);
            break;
        case 'l':
            mrubiks = rotateFace(mrubiks, LEFT, angle);
            break;
        case 'r':
            mrubiks = rotateFace(mrubiks, RIGHT, angle);
            break;
        case '1':
            resetCube();
        case '5':
            API(mrubiks);
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
