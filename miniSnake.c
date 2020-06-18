#include "strutture.h"

static CorpoSerpe head;

static CorpoSerpe* fantasma, *supporto;
#define MOV (1.0/2.0)
static int UP=1,DOWN=0,LEFT=0,RIGHT=0;

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

    //COMANDI IMPARTITI
    printf("X: vertx1=%f\t vertx2=%f\t vertx3=%f\t vertx4=%f\n",head.vertici.vertx1[0],head.vertici.vertx2[0],head.vertici.vertx3[0],head.vertici.vertx4[0]);
    printf("Y: vertx1=%f\t vertx2=%f\t vertx3=%f\t vertx4=%f\n",head.vertici.vertx1[1],head.vertici.vertx2[1],head.vertici.vertx3[1],head.vertici.vertx4[1]);
    if(UP==1){
        if(head.vertici.vertx4[1]!=5.0){
            currPos[0]+= 0.0; currPos[1]+= MOV;
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
            currPos[0]+= MOV; currPos[1]+= 0.0;
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
            currPos[0]-= 0.0; currPos[1]-= MOV;
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
            currPos[0]-= MOV; currPos[1]-= 0.0;
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
    



    //POSIZIONAMENTO DELLA TESTA DEL PITONE
    glPushMatrix();
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2d(head.vertici.vertx1[0],head.vertici.vertx1[1]);
        glVertex2d(head.vertici.vertx2[0],head.vertici.vertx2[1]);
        glVertex2d(head.vertici.vertx3[0],head.vertici.vertx3[1]);
        glVertex2d(head.vertici.vertx4[0],head.vertici.vertx4[1]);
        glEnd();
    glPopMatrix();


    //IL RESTO DEL CORPO CHE SEGUE LA TESTA
    while (q1->next!=NULL)
    {
        q1=q1->next;
        
        supporto->vertici=q1->vertici;
        q1->vertici=fantasma->vertici;
        fantasma->vertici=supporto->vertici;

        glPushMatrix();
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2d(q1->vertici.vertx1[0],q1->vertici.vertx1[1]);
        glVertex2d(q1->vertici.vertx2[0],q1->vertici.vertx2[1]);
        glVertex2d(q1->vertici.vertx3[0],q1->vertici.vertx3[1]);
        glVertex2d(q1->vertici.vertx4[0],q1->vertici.vertx4[1]);
        glEnd();
        glPopMatrix();
        
    }

    glFinish();
    glutSwapBuffers();
}

void init(){
    GLenum glErr;
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0,5.0,-5.0,5.0,-2.0,2.0);

    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv){
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    
    head.vertici.vertx1[0]=0.5; head.vertici.vertx1[1]=0.0;
    head.vertici.vertx2[0]=0.5; head.vertici.vertx2[1]=-0.5;
    head.vertici.vertx3[0]=0.0; head.vertici.vertx3[1]=0.0;
    head.vertici.vertx4[0]=0.0; head.vertici.vertx4[1]=-0.5;
    
    fantasma=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    supporto=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    
    head.next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    head.next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    head.next->next->next=(CorpoSerpe *)malloc(sizeof(CorpoSerpe));
    head.next->next->next->next=NULL;
    
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

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}