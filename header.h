#include <GL/glut.h>

typedef enum {
    DOWN,
    FRONT,
    UP,
    BACK,
    LEFT,
    RIGHT
}enumRotation;



typedef struct _Face {
    GLfloat corner[4][3];
    GLfloat color[3];
    char cColor;
} Face;

typedef struct _Cube {
    struct _Face face[6];
    char* edgesNCornersName; //will containe the partern
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

typedef struct _Premove {
    char* edgeName;
    int* preMoves;
}Premove;

Cube* translateCube(Cube* cube, GLfloat x, GLfloat y, GLfloat z);
Cube rotateCube(Cube cube, float angle ,char axe);
Rubiks rotateFace(Rubiks rubiks, char face, float angle);
GLfloat* rotateCorner(GLfloat corner[3], float radAngle, char axe);
void rotateFacePatern(Cube movedCubes[3][3],int angle, Cube result[3][3], int face);
void updateLookAt();
Face initFace();
void drawAxis();
Cube* initCube();
void showFace(Face face);
void showCube(Cube cube);
int isEqual(Rubiks rubiks, Rubiks rubiks2);
Rubiks initRubik();
Rubiks doTPermutation(Rubiks rubiks);
int* find(char* edgeName, Rubiks rubiks);
Rubiks solveEdges(Rubiks rubiks, Rubiks solvedRubiks);
int areEqual(Rubiks rubiks, Rubiks rubiks2);
Rubiks doYPermutation(Rubiks rubiks);
Rubiks solveCorners(Rubiks rubiks, Rubiks solvedRubiks);
Rubiks scrambleRubiks(Rubiks rubiks);
Rubiks solve(Rubiks rubiks, Rubiks solvedRubiks);
void resetNbMovement();
int getNbMovement();
