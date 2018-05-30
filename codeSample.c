//Mise en places des cubes du rubiks
//

    
    GLfloat transX, transY, transZ;
    transX = transY = transZ = 0;
    for(int i=0; i<3; i++){
        transX = -2.1 + 2.1*i;
        //On deplace 9 des 27 cubes à gauche et à droite des premiers cubes afin de deployer au fur et à mesure
        for(int j=0; j<9; j++){
            translateCube(cube[j+i], transX, transY, transZ);
        }
    }
//ou
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            for(k=0; k<3; k++){
                translateCube(cube[i+j+k], 2.1, 0., 0.);             
            }
            translateCube(cube[i+j+k], 0, 2.1, 0.);             
        }
        translateCube(cube[i+j+k], 0., 0., 2.1);             
    }







GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };

GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };

void
drawRubik(void)
{
    int j, k, l;
    glTranslatef(-3, -3, 0);

    for(l = 0; l < 3; l++) {
        glPushMatrix();
        for(k = 0; k < 3; k++) { 
            glPushMatrix();
            for(j = 0; j < 3; j ++) {
                drawCube(v);
                glTranslatef(2.1, 0.0, 0.0);
            }
            glPopMatrix();
            glTranslatef(0.0, 2.1, 0.0);
        }
        glPopMatrix();
        glTranslatef(0.0, 0.0, 2.1);
    }
}

void 
drawCube(GLfloat v[8][3]){
    glRasterPos3i(4,0,0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    int i;
    for (i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        glColor3f(faceColor[i][0],faceColor[i][1],faceColor[i][2]);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}




/*
    RubiksFace rubiksFace[6];
    //DOWN, every face translated on Y negativly
    rubiksFace[0].cube[0] = *cube[3]; 
    rubiksFace[0].cube[1] = *cube[4]; 
    rubiksFace[0].cube[2] = *cube[5]; 
    rubiksFace[0].cube[3] = *cube[12]; 
    rubiksFace[0].cube[4] = *cube[14]; 
    rubiksFace[0].cube[5] = *cube[20]; 
    rubiksFace[0].cube[6] = *cube[22]; 
    rubiksFace[0].cube[7] = *cube[25]; 
    rubiksFace[0].cube[8] = *cube[26]; 
    //FRONT, every face translated on X positivly
    rubiksFace[1].cube[0] = *cube[2]; 
    rubiksFace[1].cube[1] = *cube[5]; 
    rubiksFace[1].cube[2] = *cube[8]; 
    rubiksFace[1].cube[3] = *cube[15]; 
    rubiksFace[1].cube[4] = *cube[17]; 
    rubiksFace[1].cube[5] = *cube[19]; 
    rubiksFace[1].cube[6] = *cube[20]; 
    rubiksFace[1].cube[7] = *cube[23]; 
    rubiksFace[1].cube[8] = *cube[25]; 
    //UP, every face translated on Y positivly
    rubiksFace[2].cube[0] = *cube[6]; 
    rubiksFace[2].cube[1] = *cube[7]; 
    rubiksFace[2].cube[2] = *cube[8]; 
    rubiksFace[2].cube[3] = *cube[11]; 
    rubiksFace[2].cube[4] = *cube[13]; 
    rubiksFace[2].cube[5] = *cube[19]; 
    rubiksFace[2].cube[6] = *cube[21]; 
    rubiksFace[2].cube[7] = *cube[23]; 
    rubiksFace[2].cube[8] = *cube[24]; 
    //BACK, every face translated on X negativly
    rubiksFace[3].cube[0] = *cube[0]; 
    rubiksFace[3].cube[1] = *cube[3]; 
    rubiksFace[3].cube[2] = *cube[6]; 
    rubiksFace[3].cube[3] = *cube[16]; 
    rubiksFace[3].cube[4] = *cube[18]; 
    rubiksFace[3].cube[5] = *cube[21]; 
    rubiksFace[3].cube[6] = *cube[22]; 
    rubiksFace[3].cube[7] = *cube[24]; 
    rubiksFace[3].cube[8] = *cube[26]; 
    //LEFT, every face translated on Z negativly
    rubiksFace[4].cube[0] = *cube[0]; 
    rubiksFace[4].cube[1] = *cube[3]; 
    rubiksFace[4].cube[2] = *cube[6]; 
    rubiksFace[4].cube[3] = *cube[16]; 
    rubiksFace[4].cube[4] = *cube[18]; 
    rubiksFace[4].cube[5] = *cube[21]; 
    rubiksFace[4].cube[6] = *cube[22]; 
    rubiksFace[4].cube[7] = *cube[24]; 
    rubiksFace[4].cube[8] = *cube[26]; 
    //LEFT, every face translated on Z prositivly
    rubiksFace[5].cube[0] = *cube[9]; 
    rubiksFace[5].cube[1] = *cube[11]; 
    rubiksFace[5].cube[2] = *cube[12]; 
    rubiksFace[5].cube[3] = *cube[15]; 
    rubiksFace[5].cube[4] = *cube[16]; 
    rubiksFace[5].cube[5] = *cube[19]; 
    rubiksFace[5].cube[6] = *cube[20]; 
    rubiksFace[5].cube[7] = *cube[24]; 
    rubiksFace[5].cube[8] = *cube[26]; 
    */
