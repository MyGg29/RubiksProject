#include "header.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define PI 3.14159265

int verboseMoves = 1; //controle the printf
int nbMovement=0;//used to get the score
char* enumRotation2Char[6] = {
    "DOWN",
    "FRONT",
    "UP",
    "BACK",
    "LEFT",
    "RIGHT"
};
//move the coords of a cube around
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

//after a rotation of a face is done, we has a faulty representation of the cube in ours 3x3x3 matrice, we fixe it here by rotating the elements of the matrix representation the face
void rotateFacePatern(Cube movedCubes[3][3],int angle, Cube result[3][3], int face)
{
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            if(face != UP && face != DOWN) 
            {
                if(angle>0)
                    result[i][j] = movedCubes[j][3 - i - 1];
                if(angle<0)
                    result[i][j] = movedCubes[3 - j - 1][i];
            }
            if(face == UP || face == DOWN)//special case for those 2 faces, "it's a feature"
            {
                if(angle>0)
                    result[i][j] = movedCubes[3 - j - 1][i];
                if(angle<0)
                    result[i][j] = movedCubes[j][3 - i - 1];
            }
        }
    }
}

//rotate a cube around an axis. this is done by rotating every face in the cube. so we change the the coords of the points in the face
Cube rotateCube(Cube cube, float angle ,char axe)
{
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

//rotate a face around, to do this, we select every cube in a face by fixing a coordinate.Also print out in the console the move done 
Rubiks rotateFace(Rubiks rubiks, char face, float angle)
{
    nbMovement++;
    if(angle > 0)
    {
        printf("%s,",enumRotation2Char[(int)face]);
    }
    else{
        printf("%s',",enumRotation2Char[(int)face]);
    }
    //pour chaque cube d'une face ....
    Cube movedCubes[3][3];
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(face==FRONT)
            {
                rubiks.cube[0][i][j] = rotateCube(rubiks.cube[0][i][j], angle, 'x');
                movedCubes[i][j] = rubiks.cube[0][i][j];
            }
            if(face==BACK)
            {
                rubiks.cube[2][i][j] = rotateCube(rubiks.cube[2][i][j], angle, 'x');
                movedCubes[i][j] = rubiks.cube[2][i][j];
            }
            if(face==DOWN)
            {
                rubiks.cube[i][2][j] = rotateCube(rubiks.cube[i][2][j], angle, 'y');
                movedCubes[i][j] = rubiks.cube[i][2][j];
            }
            if(face==UP)
            {
                rubiks.cube[i][0][j] = rotateCube(rubiks.cube[i][0][j], angle, 'y');
                movedCubes[i][j] = rubiks.cube[i][0][j];
            }
            if(face==LEFT)
            {
                rubiks.cube[i][j][0] = rotateCube(rubiks.cube[i][j][0], angle, 'z');
                movedCubes[i][j] = rubiks.cube[i][j][0];
            }
            if(face==RIGHT)
            {
                rubiks.cube[i][j][2] = rotateCube(rubiks.cube[i][j][2], angle, 'z');
                movedCubes[i][j] = rubiks.cube[i][j][2];
            }
        }
    }
    Cube result[3][3];
   
    //important, after the rotation, the rubiks representation is wrong, we fixe this here.
    rotateFacePatern(movedCubes, angle, result, face);
    
    //can't assign a array to another array
    if(face==FRONT)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[0][i][j] = result[i][j];
    if(face==BACK)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[2][i][j] = result[i][j];
    if(face==DOWN)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[i][2][j] = result[i][j];
    if(face==UP)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[i][0][j] = result[i][j];
    if(face==LEFT)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[i][j][0] = result[i][j];
    if(face==RIGHT)
        for(int i=0; i<3;i++)
            for(int j=0; j<3;j++)
                rubiks.cube[i][j][2] = result[i][j];

    return rubiks;
}

//probably the most important function
//we get a point (corner[3]) with x,y,z coords, then we rotate it with a 3d rotation matrix
//there are 3 rotation matrix in order to rotate around 3 axes. 
//axe have to be "x", "y" or "z"
GLfloat* rotateCorner(GLfloat corner[3], float radAngle, char axe){
    GLfloat* result = malloc(sizeof(GLfloat)*3); 
	result[0] = result[1] = result[2] = 0.;
    //rotation matrix around X
	float rotationMatrixX [3][3] = { {1, 0, 0},
									{0, cos(radAngle), -sin(radAngle)},
									{0, sin(radAngle), cos(radAngle)}
								  };
    //around  Y
    float rotationMatrixY [3][3] = { {cos(radAngle), 0, sin(radAngle)},
                                     {0, 1, 0},
                                     {-sin(radAngle), 0, cos(radAngle)}
                                  };
    //around Z
    float rotationMatrixZ [3][3] = { {cos(radAngle), -sin(radAngle), 0},
                                     {sin(radAngle), cos(radAngle), 0},
                                     {0, 0, 1}
                                   };

    float rotationMatrix[3][3];
    //we chose which one we are going to usej
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

    //we multiply the points coords by the matrix, this a multiplication of matrices
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i] += rotationMatrix[i][j] * corner[j];
		}
	}
	return result;
}

//reset how manys moves we did
//is incremented in rotateFace
void resetNbMovement()
{
    nbMovement = 0;
}

//get how many moves we did since the reset
int getNbMovement()
{
    return nbMovement;
}
void setVerboseMoves(int state)
{
    verboseMoves = state;
}
