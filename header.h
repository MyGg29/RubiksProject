#include <GL/glut.h>

typedef enum {
    DOWN,
    FRONT,
    UP,
    BACK,
    LEFT,
    RIGHT
}enumPosition;

typedef struct _Face {
    GLfloat corner[4][3];
    GLfloat color[3];
    char cColor;
} Face;

typedef struct _Cube {
    struct _Face face[6];
} Cube;

typedef struct _Rubiks {
    struct _Cube cube[3][3][3];
    char* edgesNCornersPos[20]; //will containe the partern
} Rubiks;

typedef struct _FaceAPI {
    int face;//up,down,front,etc
    int color[9][3];//Une couleur par carré présent sur une face
}FaceAPI;

typedef struct _RubiksAPI {
    struct _FaceAPI face[6];//6 faces ds un cube
} RubiksAPI;

Cube* translateCube(Cube* cube, GLfloat x, GLfloat y, GLfloat z);
Cube rotateCube(Cube cube, float angle ,char axe);
Rubiks rotateFace(Rubiks rubiks, char face);
GLfloat* rotateCorner(GLfloat corner[3], float radAngle, char axe);
void rotateFacePatern(Cube movedCubes[3][3], Cube result[3][3], int face);
void updateLookAt();
Cube* drawCube();
void showFace(Face face);
void showCube(Cube cube);
