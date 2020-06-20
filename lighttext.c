#include "strutturelight.h"
#include "readBMP.h"

#define MOV (1.0/2.0)

static int UP=1,DOWN=0,LEFT=0,RIGHT=0;
static CorpoSerpe head;
static CorpoSerpe* fantasma, *supporto;
struct BitMapFile *image = NULL;
struct BitMapFile *image2 = NULL;
struct BitMapFile *image3 = NULL;
char *fileName = "snakeHead.bmp";
char *fileName2 = "scales3.bmp";
char *fileName3 = "grass.bmp";
static GLenum textureID[3];
float angle = 0;
float y = 0, x = 0;
float coords = 0;

material_t snakematerial = {
    {0.6f, 0.35f, 0.05f,0.92},{0.6,0.35,0.5,0.92},{0.7,0.7,0.7,1.0},{120.0}
    // {1.0,0.7,0.1,0.92},{1.0,0.7,0.1,0.92},{1.0,0.7,0.5,1.0},{70.0}
};
material_t groundmaterial = {
    {0.3f, 0.33f, 0.4f,0.92},{0.4,0.4,0.4,0.92},{0.7,0.7,0.7,1.0},{120.0}
    // {1.0,0.7,0.1,0.92},{1.0,0.7,0.1,0.92},{1.0,0.7,0.5,1.0},{70.0}
};

void initSnake();


void keyInput(int key, int x, int y){
    switch(key) {
        case 27:
            // ESC
            exit(0);
            break;

        case GLUT_KEY_UP:
            if(DOWN==1) break;
            else{            
                    UP=1;
            LEFT=0;         RIGHT=0;
                    DOWN=0;
            
            glutPostRedisplay();
            break;
            }
        case GLUT_KEY_DOWN:
            if(UP==1) break;
            else{            
                    UP=0;
            LEFT=0;         RIGHT=0;
                    DOWN=1;
            
            glutPostRedisplay();
            break;
            }
        case GLUT_KEY_RIGHT:
            if(LEFT==1) break; 
            else{
                    UP=0;
            LEFT=0;         RIGHT=1;
                    DOWN=0;
            
            glutPostRedisplay();
            break;
            }
        case GLUT_KEY_LEFT:
            if(RIGHT==1) break;
            else{
                    UP=0;
            LEFT=1;         RIGHT=0;
                    DOWN=0;
            
            glutPostRedisplay();
            break;
            }
        default:
            break;
    }
}



void display(void){
    CorpoSerpe* q1=&head;
    
    GLfloat currPos[2];
    glClear(GL_COLOR_BUFFER_BIT);
    GLfloat lightPos[4] = {0.0,3.0,0.5,1.0};
	/* Initialize light position */
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

    //COMANDI IMPARTITI
    printf("X: vertx1=%f\t vertx2=%f\t vertx3=%f\t vertx4=%f\n",head.vertici.vertx1[0],head.vertici.vertx2[0],head.vertici.vertx3[0],head.vertici.vertx4[0]);
    printf("Y: vertx1=%f\t vertx2=%f\t vertx3=%f\t vertx4=%f\n",head.vertici.vertx1[1],head.vertici.vertx2[1],head.vertici.vertx3[1],head.vertici.vertx4[1]);
    if(UP==1){
        if(head.vertici.vertx4[1]!=5.0){
           x+=0; y+=0.3; angle+=0;
        }else{
            head.vertici.vertx1[1]=-4.5;
            head.vertici.vertx2[1]=-5.0;
            head.vertici.vertx3[1]=-4.5;
            head.vertici.vertx4[1]=-5.0;   
            currPos[0]= 0.0; currPos[1]= 0.0; 
        }
    }
    if(RIGHT==1){
        if(head.vertici.vertx3[0]!=5.0){
            x += 0.3; y+=0; angle+=90;
        }else{
            head.vertici.vertx1[0]=-4.5;
            head.vertici.vertx2[0]=-4.5;
            head.vertici.vertx3[0]=-5.0;
            head.vertici.vertx4[0]=-5.0;
            currPos[0]= 0.0; currPos[1]= 0.0; 
        }
    }
    if(DOWN==1){
        if(head.vertici.vertx1[1]!=-5.0){
            x += 0; y+=-0.3; angle+=180;
        }else{
            head.vertici.vertx1[1]=4.5;
            head.vertici.vertx2[1]=5.0;
            head.vertici.vertx3[1]=4.5;
            head.vertici.vertx4[1]=5.0;   
            currPos[0]= 0.0; currPos[1]= 0.0; 
        }
    }
    if(LEFT==1){
        if(head.vertici.vertx2[0]!=-5.0){
           x+=-0.3; y+=0; angle+=-90;
        }else{
            head.vertici.vertx1[0]=4.5;
            head.vertici.vertx2[0]=4.5;
            head.vertici.vertx3[0]=5.0;
            head.vertici.vertx4[0]=5.0;   
            currPos[0]= 0.0; currPos[1]= 0.0; 
        }
    }
    fantasma->vertici=head.vertici;

    //SPOSTAMENTO DEL SERPENTE
    head.vertici.vertx1[0]+=currPos[0]; head.vertici.vertx1[1]+=currPos[1];
    head.vertici.vertx2[0]+=currPos[0]; head.vertici.vertx2[1]+=currPos[1];
    head.vertici.vertx3[0]+=currPos[0]; head.vertici.vertx3[1]+=currPos[1];
    head.vertici.vertx4[0]+=currPos[0]; head.vertici.vertx4[1]+=currPos[1];
    //activate heead texture
    //glBindTexture(GL_TEXTURE_2D, textureID[0]);
    //ground

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   groundmaterial.matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   groundmaterial.matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  groundmaterial.matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, groundmaterial.shine);
    
    glBindTexture(GL_TEXTURE_2D, textureID[2]);

    glBegin(GL_TRIANGLE_STRIP);
       glTexCoord2f(0,0); glVertex3f(-10,-10,-1);
       glTexCoord2f(1,0); glVertex3f(10,-10,-1);
       glTexCoord2f(0,1); glVertex3f(-10,10,-1);
       glTexCoord2f(1,1); glVertex3f(10,10,-1);
    glEnd();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   snakematerial.matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   snakematerial.matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  snakematerial.matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, snakematerial.shine);
    GLUquadric *quad;
	quad = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    //glRotated(90,0,1,0);
    //POSIZIONAMENTO DELLA TESTA DEL PITONE
    
    glPushMatrix();


        glTranslatef(x,y,0);
        glRotatef(angle,0,0,1);
        
        
        //glColor3f(1.0,0.0,0.0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0, 0.85); glVertex2d(-0.2,0.2);
            glTexCoord2f(0.0, 0.0); glVertex2d(-0.2,-0.2);
            glTexCoord2f(1.0, 0.85); glVertex2d(0.2,0.2);
            glTexCoord2f(1.0, 0.0); glVertex2d(0.2,-0.2);
        glEnd();
        glPopMatrix();
        //gluSphere(quad, 1, 20, 20);
   

    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   snakematerial.matAmbient);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   snakematerial.matDiffuse);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  snakematerial.matSpecular);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, snakematerial.shine);
    //IL RESTO DEL CORPO CHE SEGUE LA TESTA

    //activate body skin texture
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glPushMatrix();
    glTranslatef(x,y,0);
    coords = 0.4;
    while (q1->next!=NULL)
    {
        q1=q1->next;
        
        supporto->vertici=q1->vertici;
        q1->vertici=fantasma->vertici;
        fantasma->vertici=supporto->vertici;

        //glPushMatrix();
        //glColor3f(0.0,0.0,1.0);
        glBegin(GL_TRIANGLE_STRIP);
           glTexCoord2f(0.0, 0.0); glVertex3f(-0.2,0.2-coords,0);
           glTexCoord2f(1.0, 0.0); glVertex3f(-0.2,-0.2-coords,0);
           glTexCoord2f(0.0, 1.0); glVertex3f(0.2,0.2-coords,0);
           glTexCoord2f(1.0, 1.0); glVertex3f(0.2,-0.2-coords,0);
        glEnd();
        coords+=0.4;
    }
    glPopMatrix();
    glFinish();
    glutSwapBuffers();
}
void initSnake() {
    head.vertici.vertx1[0]=0.5; head.vertici.vertx1[1]=0.0;
    head.vertici.vertx2[0]=0.5; head.vertici.vertx2[1]=-0.5;
    head.vertici.vertx3[0]=0.0; head.vertici.vertx3[1]=0.0;
    head.vertici.vertx4[0]=0.0; head.vertici.vertx4[1]=-0.5;
    
    fantasma=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    supporto=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    
    head.next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    head.next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    // head.next->next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    head.next->next->next=NULL;
    head.snakeMaterial = snakematerial;

}

void init(){
    GLenum glErr;
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    initSnake();
    glGenTextures (3,textureID);
    //define texture for snake's head.
    glBindTexture (GL_TEXTURE_2D, textureID[0]);
    glTexImage2D (GL_TEXTURE_2D,0,GL_RGBA,image->sizeX,image->sizeY,0,GL_RGBA,GL_UNSIGNED_BYTE,image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //da cambiare
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //da cambiare
    //enable mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //define texutre for snake's skin
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2->sizeX, image2->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2->data);
    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture parameters for filtering.
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //define texture for grass
    glBindTexture(GL_TEXTURE_2D, textureID[2]);
    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image3->sizeX, image3->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3->data);
    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture parameters for filtering.
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
    //define lighting
    float globAmb[] = {1.0, 1.0, 1.0, 1.0};
    float lightAmb[] = {0.8, 0.8,0.8, 1.0};
	float lightDif[] = {0.3, 0.3, 0.3, 1.0};
    float lightSpec[] = {0.2, 0.2, 0.2, 1.0};

    glEnable(GL_LIGHTING);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    // Global ambient light.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);

	// Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	// Enable local viewpoint.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0,5.0,-5.0,5.0,-2.0,2.0);

    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //enable shaders
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char** argv){
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    
    // head.vertici.vertx1[0]=0.5; head.vertici.vertx1[1]=0.0;
    // head.vertici.vertx2[0]=0.5; head.vertici.vertx2[1]=-0.5;
    // head.vertici.vertx3[0]=0.0; head.vertici.vertx3[1]=0.0;
    // head.vertici.vertx4[0]=0.0; head.vertici.vertx4[1]=-0.5;
    
    // fantasma=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    // supporto=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    
    // head.next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    // head.next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    // head.next->next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    // head.next->next->next->next=NULL;
    
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(400, 400);
    glutCreateWindow ("OpenGL Window");

    glutSpecialFunc(keyInput);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("GLEW init failed: %s\n", glewGetErrorString(err));
        exit(1);
    } else {
        printf("GLEW init success\n");
    };

    image = readBMP(fileName);
    if (image == NULL) {
        printf("readBMP: Could not openfile: %s \n", fileName);
        exit(1);
    } else {
        printf("File %s has size %d x %d \n", fileName, image->sizeX, image->sizeY);
    }

    image2 = readBMP(fileName2);
    if (image2 == NULL) {
        printf("readBMP: Could not openfile: %s \n", fileName2);
        exit(1);
    } else {
        printf("File %s has size %d x %d \n", fileName2, image2->sizeX, image2->sizeY);
    }

    image3 = readBMP(fileName3);
    if (image3 == NULL) {
        printf("readBMP: Could not openfile: %s \n", fileName3);
        exit(1);
    } else {
        printf("File %s has size %d x %d \n", fileName3, image3->sizeX, image3->sizeY);
    }

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}
