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
