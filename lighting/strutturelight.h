#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
typedef struct Material {
    float	matAmbient[4],
			matDiffuse[4],
			matSpecular[4],
			shine[1];
} material_t;

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
    material_t snakeMaterial;
    struct _serpente *next;
};
typedef struct _serpente CorpoSerpe;
