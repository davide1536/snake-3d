/* TODO:
 * - Input lag dovuto a redisplay (tra glutTimerFunc e keyInput)
 * - 3D
 * - VBO
 * - Griglia
 */
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/structures.h"	// Strutture dati personalizzate necessarie
#include "lib/snake.h"		// Procedure per il serpente

// Abilita il double buffering
#define DOUBLE_BUFFER
int snakeCoordxAus = 0,snakeCoordyAus = 0; //coords for virtual matrix (snake)
int snakeCoordxGhost = 0, snakeCoordyGhost = 0; //coords for virtual matrix (fruits)
int numberFruits = 0;
float x = 0, y = 0; //parameter used in translate
int score = 0;

material_t snakematerial = {
    {0.6f, 0.35f, 0.05f,0.92},{0.6,0.35,0.5,0.92},{0.7,0.7,0.7,1.0},{120.0}
    // {1.0,0.7,0.1,0.92},{1.0,0.7,0.1,0.92},{1.0,0.7,0.5,1.0},{70.0}
};

material_t fruitMaterial = {
    {0.8f, 0.1f, 0.05f,0.92},{1.0,0.5,0.5,0.92},{0.7,0.7,0.7,1.0},{100.0}
    // {1.0,0.7,0.1,0.92},{1.0,0.7,0.1,0.92},{1.0,0.7,0.5,1.0},{70.0}
};
// Coordinate iniziali per i vertici dei cubi. (primo blocco, testa)
Cube initialBlocks[INITIAL_BLOCK_NO] = { 
	{	
		.coordx = 0,
		.coordy = 0
	},
	{	

		.coordx = 0,
		.coordy = -1
	},
	{
		
		.coordx = 0,
		.coordy = -2
	},
	{
		.coordx = 0,
		.coordy = -3
	},
	{
		.coordx = 0,
		.coordy = -4
	},
	{
		.coordx = 0,
		.coordy = -5
	}
};

// Lista collegata rappresentante il serpente.
Snake 	*head,			// Testa del serpente (primo elemento)
		*lastBlock;		// Ultimo blocco inserito

// Elemento ausiliario "fantasma" per salvare la posizione di un blocco prima dello spostamento.
Cube 	ghost,
		ausilio;
Cube snakeCube = {
	.vertex = {
        {0.25, 0.25},
        {0.25, -0.25},
        {-0.25, 0.25},
        {-0.25, -0.25}
    }
};
Cube frutto = {
    .vertex = {
        // {0.5, 0.0},
        // {0.5, -0.5},
        // {0.0, 0.0},
        // {0.0, -0.5}
		 {0.25, 0.25},
        {0.25, -0.25},
        {-0.25, 0.25},
        {-0.25, -0.25}
    }
};

// Input.
static Input userDirection;	// Direzione inserita dall'utente

// Movimento del serpente.
static GLfloat currPos[2] = {0.0, 0.0};

// Gestione tasti per uscita.
void exitHandler(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
		case 'q':
			killSnake();
			exit(0);
		default:
			break;
	}
}
void spawnCubo(void){
    float randomX,randomY;
    float vertx[2];
	frutto.snakeMaterial = fruitMaterial;


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   frutto.snakeMaterial.matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   frutto.snakeMaterial.matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  frutto.snakeMaterial.matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, frutto.snakeMaterial.shine);


	if (!numberFruits) {
    	frutto.coordx = -10+rand()%20;
    	frutto.coordy = -10+rand()%20;
		printf ("frutto: %d,%d\n",frutto.coordx,frutto.coordy);
		numberFruits ++; 
	}
	
    glPushMatrix();
    glTranslatef (frutto.coordx*CELL,frutto.coordy*CELL,0);
        glBegin(GL_TRIANGLE_STRIP);
            glColor3f(1.0,0.0,0.0);
            for(int i = 0; i < VERTEX_NO; i++)
				glVertex2f(frutto.vertex[i][0], frutto.vertex[i][1]);

        glEnd();
    glPopMatrix();
}
void checkFruit() {
	if (head->block.coordx == frutto.coordx && head->block.coordy == frutto.coordy) { //se la testa si trova nella stessa coordinata x e y mangia il frutto
		frutto.trigger=1;
		Snake *aux = malloc(sizeof(Snake));
	
		// Copia dei dati
		aux->block = snakeCube;
		
		// Imposto il nuovo elemento come ultimo
		aux->next = NULL;

		// Se la lista è vuota, imposto questo elemento come primo
		if(lastBlock != NULL)
			lastBlock->next = aux;
		lastBlock = aux;

		numberFruits --;
		score += 10;
	}
};

void checkDeath() {
	for(Snake *ptr = head->next; ptr != NULL; ptr = ptr->next) {
		if (head->block.coordx == ptr->block.coordx && head->block.coordy == ptr->block.coordy ) { //controllo se sono sulla stessa coordinata di un mio stesso cubo
			killSnake();
			exit(-1);
		}
	}
};

// Input da tastiera.
void keyInput(int key, int x, int y){
	switch(key) {
		case GLUT_KEY_UP:
			fprintf(stderr, "up\n");
			if(userDirection == down) 
				break;
			userDirection 		= up;
			break;
		case GLUT_KEY_DOWN:
			fprintf(stderr, "down\n");
			if(userDirection == up) 
				break;
			userDirection = down;
			break;
		case GLUT_KEY_RIGHT:
					fprintf(stderr, "right\n");
			if(userDirection == left) 
				break;
			userDirection 		= right;
			break;
		case GLUT_KEY_LEFT:
					fprintf(stderr, "left\n");
			if(userDirection == right) 
				break;
			userDirection = left;
			break;
		default:
			break;
	}
}

// Gestisce l'input inserito dall'utente.
void processInput() {
	/* Aggiornamento della posizione del serpente in base alle coordinate specificate
	 * dall'utente; gestione "effetto pacman"
	 */
	switch(userDirection) {
		case up:
			if( head->block.coordy<10) {    //x: di quante caselle mi devo muovere lungo l'asse x al prossimo refresh
				y = 1;						//y: di quante caselle mi devo muovere lungo l'asse y al prossimo refresh
                x = 0;
 			} else {
				y = -20;
                x = 0;
			}
			break;
		case down:    
			if( head->block.coordy > -10) {
				y = -1;
                x = 0;				
 			} else {
				y = 20;
				x =  0;
			}
			break;
		case left:
 			if( head->block.coordx > -10) {
				x = -1;
                y = 0;
				
 			} else {
				x = 20;
				y = 0;
			}
			break;
		case right:
 			if( head->block.coordx < 10) {
				x = 1;
                y = 0;
 			} else {
				x = -20;
                y = 0;
			}
			break;
		default:
			break;
	}
	//save previous head position 
	snakeCoordyGhost = head->block.coordy;
	snakeCoordxGhost = head->block.coordx;
	//update head position
    head->block.coordy += y;
    head->block.coordx += x;
	// Salvo la posizione della testa
	//ghost = head->block;

	// Scrivo le modifiche nella testa del serpente
	// for(int i = 0; i < VERTEX_NO; i++) {
	// 	for(int j = 0; j < 2; j++)
	// 		head->block.vertex[i][j] += currPos[j];
	// }
	
	// // Svuoto il buffer per non accumulare le posizioni durante i refresh
	// for(int i = 0; i < 2; i++)
	// 	currPos[i] = 0.0;
	// for( Snake *morte=head->next;morte!=NULL;morte= morte->next){
	// 	if(head->block.vertex[0][0] == morte->block.vertex[0][0] && head->block.vertex[0][1] == morte->block.vertex[0][1]){
	// 		printf("SEI MORTO BRUTTO COGLIONE!!");
	// 		exit(-1);
	// 	}
	// }
	
	// if( frutto.vertex[0][0] == head->block.vertex[0][0] && frutto.vertex[0][1] == head->block.vertex[0][1]){
	// 	frutto.trigger=1;
	// 	Snake *aux = malloc(sizeof(Snake));
	
	// 	// Copia dei dati
	// 	aux->block = frutto;
		
	// 	// Imposto il nuovo elemento come ultimo
	// 	aux->next = NULL;

	// 	// Se la lista è vuota, imposto questo elemento come primo
	// 	if(lastBlock != NULL)
	// 		lastBlock->next = aux;
	// 	lastBlock = aux;
	// }


}

void killSnake() {
	Snake *ptr = head;
	// Deallocazione di tutti i blocchi del serpente
	while(ptr != NULL) {
		Snake* aux = ptr;
		ptr = ptr->next;
		free(aux);
	}
}

void drawSnake() {
	int rot = 0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   head->block.snakeMaterial.matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   head->block.snakeMaterial.matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  head->block.snakeMaterial.matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, head->block.snakeMaterial.shine);
	for(Snake *ptr = head; ptr != NULL; ptr = ptr->next) {
		if(ptr == head) {
			glColor3f(0.0, 0.0, 1.0);
			ptr->block.coordx; 
			ptr->block.coordy;
			// printf ("testa: %d,%d\n",ptr->block.coordx,ptr->block.coordy);
			// snakeCoordyGhost = head->block.coordy;
			// snakeCoordxGhost = head->block.coordx;
			rot = 1;
		} 
		else {
			glColor3f(0.0, 1.0, 0.0);
			//Effettuo lo scambio di variabili
			// ausilio 	= ptr->block; 
			// ptr->block 	= ghost;
			// ghost 		= ausilio;

			//x
			snakeCoordxAus = ptr->block.coordx;
			ptr->block.coordx = snakeCoordxGhost;
			snakeCoordxGhost = snakeCoordxAus;

			//y
			snakeCoordyAus = ptr->block.coordy;
			ptr->block.coordy = snakeCoordyGhost;
			snakeCoordyGhost = snakeCoordyAus;
			// printf ("corpo: %d,%d\n",ptr->block.coordx,ptr->block.coordy);

		}
		glPushMatrix();
		glTranslatef (ptr->block.coordx*CELL,ptr->block.coordy*CELL,0); 
		if (rot == 1)
			glRotatef(90,0,0,1);
		glBegin(GL_TRIANGLE_STRIP);
			// Specifico i vertici da disegnare nella scena
			for(int i = 0; i < VERTEX_NO; i++)
				glVertex2f(ptr->block.vertex[i][0], ptr->block.vertex[i][1]);
		glEnd();
		glPopMatrix();
	}
}

void appendBlock(int index) {
	// Allocazione nuovo elemento
	Snake *aux = malloc(sizeof(Snake));
	aux->block.trigger=0;
	// Copia dei dati
	aux->block = snakeCube;
	aux->block.coordx = initialBlocks[index].coordx;
	aux->block.coordy = initialBlocks[index].coordy; 

	
	// Imposto il nuovo elemento come ultimo
	aux->next = NULL;

	// Se la lista è vuota, imposto questo elemento come primo
	if(lastBlock != NULL)
		lastBlock->next = aux;
	lastBlock = aux;  
}
void writeBitmapString(void *font, char *string) {
	char *c;
   	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
void writeScore() {
	char stringScore[25];
	int xText,yText;

	xText = 8;
	yText = 9;

	glRasterPos3f(xText*CELL,yText*CELL,0);
	glPushMatrix();
	// glTranslatef (xText*CELL,yText*CELL,0);
	snprintf (stringScore,25,"score : %d",score);
    writeBitmapString(GLUT_BITMAP_8_BY_13,stringScore);
	glPopMatrix();
};

// Funzione helper per evitare warning su glutTimerFunc
void helper(int val) {
	glutPostRedisplay();
}

// Inizializzazione della lista collegata del serpente.
void snakeInit() {
	// Inizializzazione lista vuota
	head 		= NULL;
	lastBlock 	= NULL;

	// Inizializzazione elemento ausiliario
	// ghost 		= NULL;
	
	// Inserimento di un blocco in coda
	for(int i  = 0; i < INITIAL_BLOCK_NO; i++) {
		appendBlock(i);
		
		// Salvo la posizione della testa
		if(i == 0)
			head = lastBlock;
	}
	head->block.snakeMaterial = snakematerial;
	// Inizializzazione direzione iniziale
	userDirection 	 = right;
}
void initLight() {
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
};

void display() {
	GLfloat lightPos[4] = {0.0,3.0,0.5,1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	processInput();

	/* Initialize light position */
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	spawnCubo();


	//Disegno il serpente
	drawSnake();
	checkFruit();
	
	checkDeath();

	writeScore();
	// movate
	glFlush();
	glFinish();

	#ifdef DOUBLE_BUFFER
		glutSwapBuffers();
	#endif

	// Animazione del serpente
	glutTimerFunc(4000/60, helper, 0);
}

void init() {
	GLenum glErr;
	initLight();
	// Inizializzazione serpente
	snakeInit();

	// Sfondo
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Proiezione ortogonale
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0,5.0,-5.0,5.0,-2.0,2.0);

	// Controllo errori
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d\n", glErr);
        exit(-1);
    }

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char** argv) {
	GLenum glewErr;
	
	frutto.trigger=0;

	glutInit(&argc, argv);

	#ifdef DOUBLE_BUFFER
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	#else
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	#endif

	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(800, 800);
	glutCreateWindow("Snake 3D");


	glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(glewErr));
		exit(1);
	}
	
	glutSpecialFunc(keyInput);
	glutKeyboardFunc(exitHandler);
	
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}