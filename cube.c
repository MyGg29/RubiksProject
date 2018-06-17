#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "header.h"

#define PI 3.14159265

int verbose = 0;
double camPosR = 9, camPosTheta = 0, camPosPhi = 0;
Rubiks mrubiks;//the rubiks we are
Rubiks msolvedRubiks;//the rubiks we aim to be

//set up every cubes in the rubiks. Basicly we create 27 cubes in the center then translate them away to the right positions. We also init other info about the cube position in the rubiks
//this does the final init step. In the end, we have a struct with a 3x3x3 matrix inside, we also named cubes to be able to find where the should go.
//front top left is the [0][0][0]
//back down right is the [2][2][2]
Rubiks initRubik() {
    Cube* cube[27];
    Rubiks* rubiks = malloc(sizeof(Rubiks));

    for(int i=0; i<27; i++){
        cube[i] = initCube();//Init the cubes;
    }
   
    //place them, translateCube doc is in movement.c
    //pretty big chunk, can probably be refactored. But this is clear : we have 27 cube, we move call translateCube 27 times
    //center
    translateCube(cube[0], -2.1, 0, 0);
    rubiks->cube[2][1][1] = *cube[0];//blue center
    rubiks->cube[2][1][1].edgesNCornersName = "B";
    translateCube(cube[4], 0, -2.1, 0);
    rubiks->cube[1][2][1] = *cube[4];//yellow center
    rubiks->cube[1][2][1].edgesNCornersName = "Y";
    translateCube(cube[9], 0, 0, 2.1);
    rubiks->cube[1][1][0] = *cube[9];//Green center
    rubiks->cube[1][1][0].edgesNCornersName = "G";
    translateCube(cube[10], 0, 0, -2.1);
    rubiks->cube[1][1][2] = *cube[10];//Red center
    rubiks->cube[1][1][2].edgesNCornersName = "R";
    translateCube(cube[7], 0, 2.1, 0);
    rubiks->cube[1][0][1] = *cube[7];//white center 
    rubiks->cube[1][0][1].edgesNCornersName = "W";
    translateCube(cube[2], 2.1, 0, 0);
    rubiks->cube[0][1][1] = *cube[2];//centre orange
    rubiks->cube[0][1][1].edgesNCornersName = "O";
    
    translateCube(cube[1], 0, 0, 0);//does nothing
    rubiks->cube[1][1][1] = *cube[1];//center (not visible)
    rubiks->cube[1][1][1].edgesNCornersName = "C";

    translateCube(cube[3], -2.1, -2.1, 0);
    rubiks->cube[2][2][1] = *cube[3];
    rubiks->cube[2][2][1].edgesNCornersName = "DB";
    
    translateCube(cube[5], 2.1, -2.1, 0);
    rubiks->cube[0][2][1] = *cube[5];
    rubiks->cube[0][2][1].edgesNCornersName = "DF";

    translateCube(cube[6], -2.1, 2.1, 0);
    rubiks->cube[2][0][1] = *cube[6];
    rubiks->cube[2][0][1].edgesNCornersName = "UB";

    translateCube(cube[8], 2.1, 2.1, 0);
    rubiks->cube[0][0][1] = *cube[8];
    rubiks->cube[0][0][1].edgesNCornersName = "UF";

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

//init a cube by getting 6 faces in the right position, also init some color related stuff
Cube* initCube(){
    //get 6 faces
    Face bottomFace = initFace();
	Face frontFace = initFace();
	Face upFace = initFace();
	Face backFace = initFace();
	Face leftFace = initFace();
	Face rightFace = initFace();
	
    //rotate the 6 faces 1 by 1 to make a cube
    //to rotate 1 face we rotate 4 corners
    //we have to rotate 5 face because 1 face is already in the right pos (from the init)
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
    //bleu
    frontFace.color[0] = 0;
    frontFace.color[1] = 0;
    frontFace.color[2] = 255;
    //blanc
    upFace.color[0] = 255;
    upFace.color[1] = 255;
    upFace.color[2] = 0;
    //vert
    backFace.color[0] = 0;
    backFace.color[1] = 255;
    backFace.color[2] = 0;
    //blue
    leftFace.color[0] = 255;
    leftFace.color[1] = 0;
    leftFace.color[2] = 0;
    //orange
    rightFace.color[0] = 255;
    rightFace.color[1] = 153;
    rightFace.color[2] = 51;


    //send the cube back, but we have to allocate it for it to stay alive :)
    Cube* cube = malloc(sizeof(Cube));
    cube->face[0] = upFace; 
    cube->face[1] = rightFace;
    cube->face[2] = bottomFace;
    cube->face[3] = leftFace;
    cube->face[4] = frontFace;
    cube->face[5] = backFace;
    
    return cube;
}

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

//1 rubiks = 27 cubes
void showRubiks(Rubiks rubiks)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<3; k++)
            {
                if(i!=2 || j!=2 || k != 2) //Useful
                showCube(rubiks.cube[i][j][k]);
            }
        }
    }
}

//1 cube = 6 faces
void showCube(Cube cube){
    for(int i=0; i<6; i++){
        showFace(cube.face[i]);
    }
}

//show a shape made of 4 corners, glVertex3f to set a corner, we get the corner coordinates in the structure we have set before
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

//Generate random numbers. First we get how many moves will be done to scramble, then what the moves will be.
Rubiks scrambleRubiks(Rubiks rubiks)
{
    char* enumRotation2Char[6] = {
        "DOWN",
        "FRONT",
        "UP",
        "BACK",
        "LEFT",
        "RIGHT"
    };
    srand(time(NULL));//init the rand generator 

    int nbMoves = (rand() % 50)+20;//between 20 and 69 scramble move
    //if(verbose == 1)
        //printf("\nScramble sequence : \n"); 
    float angle = PI/2;//we could randomize this too, but it's ok I guess
    for(int i=0;i<nbMoves; i++)
    {
        int move = rand() % 6;  
    //    printf("%s,",enumRotation2Char[move]);
        rubiks = rotateFace(rubiks, move,angle);  
    }
    //printf("\n");
    fflush(stdout);

    return rubiks;
}

//Entry function for the solver to act. Had no time to do fancy stuff
void API(Rubiks rubiks)
{
    resetNbMovement();//the number of movements the rubiks made (incremented in movement.c)
    printf("SolvingSequence : \n");
    clock_t begin = clock();
    mrubiks = solve(mrubiks, msolvedRubiks);
    clock_t end = clock();
    printf("n\"Resolu\" en %f s", ((double)end-begin)/CLOCKS_PER_SEC);
    printf("\n\"Resolu\" en %d movements", getNbMovement());
    fflush(stdout);
}

//do stats
void stats()
{
    int n = 100;//the population 
    double aveElapsedTime = 0;
    int aveNbMove = 0;
    setVerbose(0);
    
    setVerboseMoves(0);
    for(int i=0;i<n;i++)
    {
        resetNbMovement();//the number of movements the rubiks made (incremented in movement.c)
        resetCube();
        scrambleRubiks(mrubiks);
        clock_t begin = clock();
        mrubiks = solve(mrubiks, msolvedRubiks);
        clock_t end = clock();
        aveElapsedTime += (((double)end-begin)/CLOCKS_PER_SEC);
        aveNbMove += getNbMovement();
    }
    printf("\n%d resolutions en %f s en moyennes\n",n, aveElapsedTime/n);
    printf("\n%d resolutions en %d movements en moyennes\n",n, aveNbMove/n);
}

//reset the global cube
void resetCube()
{
    mrubiks = initRubik();
}



void display(void)
{
    //TODO DRAW = INIT ???
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showRubiks(mrubiks);
    drawAxis();
    glutSwapBuffers();
}

void
init(void)
{
   mrubiks = initRubik();
   msolvedRubiks = initRubik();
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
    updateLookAt();
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
            break;
        case '5':
            API(mrubiks);
            break;
        case '6':
            mrubiks = scrambleRubiks(mrubiks);
            break;
        case '9':
            stats();
    }
    glutPostRedisplay();
}

//just for the sake of knowing where i am, useful when developping
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

//place the camera
void updateLookAt()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosR * cos(camPosTheta) * cos(camPosPhi)//x,y,z coords of the camera
            ,camPosR * sin(camPosTheta)
            , camPosR * cos(camPosTheta)* sin(camPosPhi),  
            0.0, 0.0, 0.0,      //where the camera is looking
            0.0, 1.0, 0.0);      //where is up
}

void setVerbose(int state)
{
    verbose = state;
}

int main(int argc, char **argv)
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
  return 0;    
}
