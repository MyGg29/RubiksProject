#include <string.h>
#include <stdio.h>
#include "header.h"

#include <unistd.h>
#define PI 3.14159265

//those 2 are global because it's hard af to pass array of array of arrays in args 
char* medgesDone[22] = {"FL","0","FR","0","DF","0","UF","0","UL","0","DL","0","DR","0","UB","0","BL","0","BR","0","DB","0"};
char* mcornersDone[14] = {"ULF","0", "DFL","0", "URB","0", "DBR","0", "UFR","0", "DRF", "0","DLB","0"};

Rubiks solve(Rubiks rubiks, Rubiks solvedRubiks)
{
    //we reset the advancement
    //I need a function who check whichs edges are already done
    char* edgesDone[22] = {"FL","0","FR","0","DF","0","UF","0","UL","0","DL","0","DR","0","UB","0","BL","0","BR","0","DB","0"};
    char* cornersDone[14] = {"ULF","0", "DFL","0", "URB","0", "DBR","0", "UFR","0", "DRF", "0","DLB","0"};

    for(int i=0;i<22;i++)
        medgesDone[i] = edgesDone[i];
    for(int i=0;i<14;i++)
        mcornersDone[i] = cornersDone[i];

    //do stuff until the edges/corners are solved
    //I do the corners first
    int edgesSolved=0; int cornersSolved=0;
    while(cornersSolved==0)
    {
        cornersSolved=1;
        for(int i=1;i<14;i=i+2)
            if(strcmp(mcornersDone[i],"0")==0)
                cornersSolved = 0;
        rubiks = solveCorners(rubiks, solvedRubiks);
    }
    while(edgesSolved==0 )
    {
        edgesSolved=1;
        for(int i=1;i<22;i=i+2)
            if(strcmp(medgesDone[i],"0")==0)
                edgesSolved = 0;
    
        rubiks = solveEdges(rubiks, solvedRubiks);
    }
    return rubiks;
} 

//we have UBL as buffer and we swap what in the buffer with what in front of the buffer
//we do preMoves first to put the cube we want to place in front of the buffer, we do the algorithm, then we do the premoves in reversed order
Rubiks solveCorners(Rubiks rubiks, Rubiks solvedRubiks)
{
    char* cornersName[7] = {"ULF", "DFL", "URB", "DBR", "UFR", "DRF", "DLB"};//UBL is the buffer
    int* cornersPreMoves[7] = {
        (int[2]){FRONT, -1},
        (int[4]){FRONT, 1, FRONT, 1},
        (int[2]){RIGHT, -1},
        (int[4]){RIGHT, 1, RIGHT, 1},
        (int[0]){},
        (int[2]){FRONT,1},
        (int[6]){DOWN,1,DOWN,1,RIGHT,1}
    };
    int premovesSize[7] = {2,4,2,4,0,2,6};
    
    int* rightBufferPos;
    char* inBuffer = rubiks.cube[2][0][0].edgesNCornersName;//what in the buffer
    rightBufferPos = find(inBuffer, solvedRubiks);//Where buffer is supposed to be

    int cornersSolved=1;
    for(int i=1;i<14;i=i+2)
    {
        if(strcmp(mcornersDone[i], "0") == 0)//if not done
            cornersSolved = 0;//not solved
    }
    if(cornersSolved == 1)//Edges are done :)
        return rubiks;

    if(*(rightBufferPos) != 2 || *(rightBufferPos+1) != 0 || *(rightBufferPos+2) != 0)//the buffer isn't blocked 
    {
        int index=0;
        for(index=0;strcmp(cornersName[index],inBuffer)!=0;index++);//We get the index of the buffer in edgesName, needed to find the list of premoves

        //apply the premoves
        for(int i=0; i<premovesSize[index]; i=i+2)
        {
            float angle = PI/2*cornersPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, cornersPreMoves[index][i],angle);
        }

        rubiks = doYPermutation(rubiks);

        //Do the opposites of the premoves
        for(int i=premovesSize[index]-2; i>=0; i=i-2)
        {
            float angle = -PI/2*cornersPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, cornersPreMoves[index][i],angle);
        }

        mcornersDone[index*2+1] = "1";
    }
    else//the buffer is blocked
    {
        int index;
        for(index=1;(strcmp(mcornersDone[index],"0")!=0) && (index<21); index = index+2);//find an edge not done.
        index = (index-1)/2;
        for(int i=0; i<premovesSize[index]; i=i+2)
        {
            float angle = PI/2*cornersPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, cornersPreMoves[index][i],angle);
        }

        rubiks = doYPermutation(rubiks);

        for(int i=premovesSize[index]-2; i>=0; i=i-2)
        {
            float angle = -PI/2*cornersPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, cornersPreMoves[index][i],angle);
        }
    }
    return rubiks;
}

//same as solveCorners but with edges. The only thing that change are edgesName, edgesPreMoves and premovesSize, will refactor the 2 functions in one when I have time
Rubiks solveEdges(Rubiks rubiks, Rubiks solvedRubiks)
{
    char* edgesName[11] = {"FL","FR","DF","UF","UL","DL","DR","UB","BL","BR","DB"};//UR is the buffer
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

    int* rightBufferPos;
    char* inBuffer = rubiks.cube[1][0][2].edgesNCornersName;//what in the buffer
    rightBufferPos = find(inBuffer, solvedRubiks);//Where buffer is supposed to be

    int edgesSolved=1;
    for(int i=1;i<22;i=i+2)
    {
        if(strcmp(medgesDone[i], "0") == 0)//if not done
            edgesSolved = 0;//not solved
    }
    if(edgesSolved == 1)//Edges are done :)
        return rubiks;

    if(*(rightBufferPos) != 1 || *(rightBufferPos+1) != 0 || *(rightBufferPos+2) != 2)//the buffer isn't blocked 
    {
        int index=0;
        for(index=0;strcmp(edgesName[index],inBuffer)!=0;index++);//We get the index of the buffer in edgesName, needed to find the list of premoves

        //apply the premoves
        for(int i=0; i<premovesSize[index]; i=i+2)
        {
            float angle = PI/2*edgesPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, edgesPreMoves[index][i],angle);
        }

        rubiks = doTPermutation(rubiks);

        //Do the opposites of the premoves
        for(int i=premovesSize[index]-2; i>=0; i=i-2)
        {
            float angle = -PI/2*edgesPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, edgesPreMoves[index][i],angle);
        }

        medgesDone[index*2+1] = "1";
    }
    else//the buffer is blocked
    {
        int index;
        for(index=1;(strcmp(medgesDone[index],"0")!=0) && (index<21); index = index+2);//find an edge not done.
        index = (index-1)/2;
        for(int i=0; i<premovesSize[index]; i=i+2)
        {
            float angle = PI/2*edgesPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, edgesPreMoves[index][i],angle);
        }

        rubiks = doTPermutation(rubiks);

        for(int i=premovesSize[index]-2; i>=0; i=i-2)
        {
            float angle = -PI/2*edgesPreMoves[index][i+1];
            rubiks = rotateFace(rubiks, edgesPreMoves[index][i],angle);
        }
    }
    return rubiks;
}

//swap the buffer UBL with UFR
Rubiks doYPermutation(Rubiks rubiks)
{
    //Y permutation
    //F' R U R’ U R U' R’ F R U' R’ U R’ F' R F 
    rubiks = rotateFace(rubiks, FRONT, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, FRONT, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, FRONT, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, FRONT, PI/2);
    return rubiks;
}

//swap the buffer UR with UL
Rubiks doTPermutation(Rubiks rubiks)
{
    //T permutation
    //R U' R’ U R’ F' R2 U R’ U R U' R’ F 
    //from https://ruwix.com/the-rubiks-cube/how-to-solve-the-rubiks-cube-blindfolded-tutorial/ with changes because my F is the site F'. Same between U and U'
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, FRONT, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, UP, PI/2);
    rubiks = rotateFace(rubiks, RIGHT, PI/2);
    rubiks = rotateFace(rubiks, UP, -PI/2);
    rubiks = rotateFace(rubiks, RIGHT, -PI/2);
    rubiks = rotateFace(rubiks, FRONT, PI/2);
    return rubiks;
}

//gives back the pos of an edge in the 3x3x3 matrix
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

//are the 2 rubiks pos equal? Not really useful because we do the edges, we check if the edges are done, then the corners and check for the forner then
int areEqual(Rubiks rubiks, Rubiks rubiks2)
{
    int equal = 1;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                if(rubiks.cube[i][j][k].edgesNCornersName != rubiks2.cube[i][j][k].edgesNCornersName)
                    equal = 0;
    return equal;
}
