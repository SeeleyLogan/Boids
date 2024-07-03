#include <Color.h>

void glClearColorI(GLint red, GLint green, GLint blue, GLint alpha)
{
    GLfloat redf   = (GLfloat) red   / 255.0f;
    GLfloat greenf = (GLfloat) green / 255.0f;
    GLfloat bluef  = (GLfloat) blue  / 255.0f;
    GLfloat alphaf = (GLfloat) alpha / 255.0f;

    glClearColor(redf, greenf, bluef, alphaf);
}
