#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

struct _quadrato
{
    GLfloat vertx1[2];
    GLfloat vertx2[2];
    GLfloat vertx3[2];
    GLfloat vertx4[2];    
};
typedef struct _quadrato Quadrato;

struct _serpente
{
    Quadrato vertici;
    int trigger;    
    struct _serpente *next;
};
typedef struct _serpente CorpoSerpe;
