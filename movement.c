#include "header.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265

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

void rotateFacePatern(Cube movedCubes[3][3], Cube result[3][3], int face)
{
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            if(face != UP && face != DOWN) //cas particulier pour 2 faces
            {
                result[i][j] = movedCubes[j][3 - i - 1];
            }
            if(face == UP || face == DOWN)
            {
                result[i][j] = movedCubes[3 - j - 1][i];
            }
        }
    }
}

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
    Cube result[3][3];
    
    rotateFacePatern(movedCubes, result, face);
    
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

    return rubiks;
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
