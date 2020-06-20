/* TODO:
 * - Input lag dovuto a redisplay (tra glutTimerFunc e keyInput)
 * - 3D
 * - VBO
 * - Griglia
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"	// Strutture dati personalizzate necessarie
#include "snake.h"		// Procedure per il serpente

// Abilita il double buffering
#define DOUBLE_BUFFER

// Coordinate iniziali per i vertici dei cubi. (primo blocco, testa)
Cube initialBlocks[INITIAL_BLOCK_NO] = { 
	{	
		.vertex = {	
			{0.5, 0.0},
			{0.5, -0.5},
			{0.0, 0.0},
			{0.0, -0.5}
		}
	},
	{	
		.vertex = {	
			{0.0, 0.0},
			{0.0, -0.5},
			{-0.5, 0.0},
			{-0.5, -0.5}
		}
	},
	{
		.vertex = {
			{-0.5, 0.0},
			{-0.5, -0.5},
			{-1.0, 0.0},
			{-1.0, -0.5}
		}
	},
	{
		.vertex = {
			{-1.0, 0.0},
			{-1.0, -0.5},
			{-1.5, 0.0},
			{-1.5, -0.5}
		}
	}
};

// Lista collegata rappresentante il serpente.
Snake 	*head,			// Testa del serpente (primo elemento)
		*lastBlock;		// Ultimo blocco inserito

// Elemento ausiliario "fantasma" per salvare la posizione di un blocco prima dello spostamento.
Cube 	ghost,
		ausilio;

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
			if(head->block.vertex[3][1] < 5.0) {
				currPos[0] += 0.0;
				currPos[1] += MOV;
 			} else {
				head->block.vertex[0][1] = -4.5;
				head->block.vertex[1][1] = -5.0;
				head->block.vertex[2][1] = -4.5;
				head->block.vertex[3][1] = -5.0;
			}
			break;
		case down:    
			if(head->block.vertex[0][1] > -5.0) {
				currPos[0] -= 0.0;
				currPos[1] -= MOV;
 			} else {
				head->block.vertex[0][1] = 4.5;
				head->block.vertex[1][1] = 5.0;
				head->block.vertex[2][1] = 4.5;
				head->block.vertex[3][1] = 5.0;
			}
			break;
		case left:
 			if(head->block.vertex[1][0] > -5.0) {
				currPos[0] -= MOV;
				currPos[1] -= 0.0;
 			} else {
				head->block.vertex[0][0] = 4.5;
				head->block.vertex[1][0] = 4.5;
				head->block.vertex[2][0] = 5.0;
				head->block.vertex[3][0] = 5.0;
			}
			break;
		case right:
 			if(head->block.vertex[2][0] < 5.0) {
				currPos[0] += MOV;
				currPos[1] += 0.0;
 			} else {
				head->block.vertex[0][0] = -4.5;
				head->block.vertex[1][0] = -4.5;
				head->block.vertex[2][0] = -5.0;
				head->block.vertex[3][0] = -5.0;
			}
			break;
		default:
			break;
	}
	
	// Salvo la posizione della testa
	ghost = head->block;

	// Scrivo le modifiche nella testa del serpente
	for(int i = 0; i < VERTEX_NO; i++) {
		for(int j = 0; j < 2; j++)
			head->block.vertex[i][j] += currPos[j];
	}
	
	// Svuoto il buffer per non accumulare le posizioni durante i refresh
	for(int i = 0; i < 2; i++)
		currPos[i] = 0.0;
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
	for(Snake *ptr = head; ptr != NULL; ptr = ptr->next) {
		if(ptr == head) {
			glColor3f(0.0, 0.0, 1.0);
		} else {
			glColor3f(0.0, 1.0, 0.0);
			// Effettuo lo scambio di variabili
			ausilio 	= ptr->block;
			ptr->block 	= ghost;
			ghost 		= ausilio;
		}
		glPushMatrix();
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
	
	// Copia dei dati
	aux->block = initialBlocks[index];
	
	// Imposto il nuovo elemento come ultimo
	aux->next = NULL;

	// Se la lista è vuota, imposto questo elemento come primo
	if(lastBlock != NULL)
		lastBlock->next = aux;
	lastBlock = aux;
}

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
	
	// Inizializzazione direzione iniziale
	userDirection 	 = right;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	processInput();

	// Disegno il serpente
	drawSnake();
	
	// movate
	glFlush();
	glFinish();

	#ifdef DOUBLE_BUFFER
		glutSwapBuffers();
	#endif

	// Animazione del serpente
	glutTimerFunc(10000/60, helper, 0);
}

void init() {
	GLenum glErr;

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv) {
	GLenum glewErr;
	
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