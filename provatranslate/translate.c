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
#include "lib/structures.h"	// Strutture dati personalizzate necessarie
#include "lib/snake.h"		// Procedure per il serpente

// Abilita il double buffering
#define DOUBLE_BUFFER
int snakeCoordxAus = 0,snakeCoordyAus = 0; //coords for virtual matrix (snake)
int snakeCoordxGhost = 0, snakeCoordyGhost = 0; //coords for virtual matrix (fruits)
float x = 0, y = 0; //parameter used in translate

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
        {0.5, 0.0},
        {0.5, -0.5},
        {0.0, 0.0},
        {0.0, -0.5}
    }
};
Cube frutto = {
    .vertex = {
        {0.5, 0.0},
        {0.5, -0.5},
        {0.0, 0.0},
        {0.0, -0.5}
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
	// frutto.vertex[0][0]=4.0;		frutto.vertex[0][1]=4.0;
	// frutto.vertex[1][0]=4.0-0.5;	frutto.vertex[1][1]=4.0;
	// frutto.vertex[2][0]=4.0;		frutto.vertex[2][1]=4.0-0.5;
	// frutto.vertex[3][0]=4.0-0.5; 	frutto.vertex[3][1]=4.0-0.5;

    /*randomX= (float)rand()/RAND_MAX;
    randomY= (float)rand()/RAND_MAX;
    
    if(randomX>=0.00 && randomX<=0.09) vertx[0]=-4.0;
    if(randomX>=0.10 && randomX<=0.19) vertx[0]=-3.0;
    if(randomX>=0.20 && randomX<=0.29) vertx[0]=-2.0;
    if(randomX>=0.30 && randomX<=0.39) vertx[0]=-1.0;
    if(randomX>=0.40 && randomX<=0.49) vertx[0]=0.0;

    if(randomX>=0.50 && randomX<=0.59) vertx[0]=1.0;
    if(randomX>=0.60 && randomX<=0.69) vertx[0]=2.0;
    if(randomX>=0.70 && randomX<=0.79) vertx[0]=3.0;
    if(randomX>=0.80 && randomX<=0.89) vertx[0]=4.0;
    if(randomX>=0.90 && randomX<=0.99) vertx[0]=5.0;



    if(randomY>=0.00 && randomY<=0.09) vertx[1]=-4.0;
    if(randomY>=0.10 && randomY<=0.19) vertx[1]=-3.0;
    if(randomY>=0.20 && randomY<=0.29) vertx[1]=-2.0;
    if(randomY>=0.30 && randomY<=0.39) vertx[1]=-1.0;
    if(randomY>=0.40 && randomY<=0.49) vertx[1]=0.0;

    if(randomY>=0.50 && randomY<=0.59) vertx[1]=1.0;
    if(randomY>=0.60 && randomY<=0.69) vertx[1]=2.0;
    if(randomY>=0.70 && randomY<=0.79) vertx[1]=3.0;
    if(randomY>=0.80 && randomY<=0.89) vertx[1]=4.0;
    if(randomY>=0.90 && randomY<=0.99) vertx[1]=5.0;
	*/
    frutto.coordx = 5;
    frutto.coordy = 0;
    glPushMatrix();
    glTranslatef (frutto.coordx*CELL,frutto.coordy*CELL,0);
        glBegin(GL_TRIANGLE_STRIP);
            glColor3f(1.0,0.0,0.0);
            for(int i = 0; i < VERTEX_NO; i++)
				glVertex2f(frutto.vertex[i][0], frutto.vertex[i][1]);

        glEnd();
    glPopMatrix();
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
			if( head->block.coordy<10) { 
				y = 1;
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
	for(Snake *ptr = head; ptr != NULL; ptr = ptr->next) {
		if(ptr == head) {
			glColor3f(0.0, 0.0, 1.0);
			ptr->block.coordx; 
			ptr->block.coordy;
			printf ("testa: %d,%d\n",ptr->block.coordx,ptr->block.coordy);
			// snakeCoordyGhost = head->block.coordy;
			// snakeCoordxGhost = head->block.coordx;
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
			printf ("corpo: %d,%d\n",ptr->block.coordx,ptr->block.coordy);

		}
		glPushMatrix();
		glTranslatef (ptr->block.coordx*CELL,ptr->block.coordy*CELL,0); 
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
	if(frutto.trigger!=1)
		spawnCubo();

	//Disegno il serpente
	drawSnake();
	
	// movate
	glFlush();
	glFinish();

	#ifdef DOUBLE_BUFFER
		glutSwapBuffers();
	#endif

	// Animazione del serpente
	glutTimerFunc(5000/60, helper, 0);
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