#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/structures.h"	// Strutture dati personalizzate necessarie
#include "lib/snake.h"		// Procedure per il serpente

// Coordinate ausiliarie per la griglia virtuale (serpente).
int snakeAuxCoords[2] = {0, 0};
// Coordinate ausiliarie per la griglia virtuale (serpente fantasma).
int ghostAuxCoords[2] = {0, 0};

// Coordinate di spostamento della testa del serpente.
float xMov = 0, 
	  yMov = 0;

// Coordinate iniziali nella griglia per i cubi.
int initialBlocks[INITIAL_BLOCK_NO][2] = { 
	{0, 0},
	{0, -1},
	{0, -2},
	{0, -3},
	{0, -4},
	{0, -5}
};

// Vertici dei cubi del serpente della scena.
GLfloat cubeVertexArray[VERTEX_NO][3] = {
	{0.5, 0.0, 0.0},
	{0.5, -0.5, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, -0.5, 0.0}
};

// Vertici dei frutti della scena.
GLfloat fruitVertexArray[VERTEX_NO][3] = {
	{0.5, 0.0, 0.0},
	{0.5, -0.5, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, -0.5, 0.0}
};

// Colori del serpente della scena.
GLfloat cubeColorArray[VERTEX_NO][3] = {
	//{0.0, 0.0, 1.0},	// Testa
	{0.0, 1.0, 0.0},	// Corpo
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0}
};

// Colori dei frutti della scena.
GLfloat fruitColorArray[VERTEX_NO][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0}
};

// Lista collegata rappresentante il serpente.
Snake 	*head,			// Testa del serpente (primo elemento)
		*lastBlock;		// Ultimo blocco inserito

// Frutto della scena.
Cube 	fruit;

// Direzione inserita dall'utente.
static Input userDirection;

// VBO/VAO stuff
unsigned int vao[2];

unsigned int cubeBuffers[2];
unsigned int fruitBuffers[2];
GLint cubeVertexIndices[VERTEX_NO];
GLint fruitVertexIndices[VERTEX_NO];

int main(int argc, char** argv) {
	GLenum glewErr;
	
	glutInit(&argc, argv);

	// Double buffering
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Definizione proprieta' finestra principale
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(800, 800);
	glutCreateWindow("Snake 3D");

	// Inizializzazione glew
	glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(glewErr));
		exit(1);
	}
	
	// Gestione input da tastiera:
	glutSpecialFunc(keyInput);		// tasti speciali (GLUT_KEY_*) per movimento
	glutKeyboardFunc(exitHandler);	// tasti (ESC...) per uscita
	
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}

// Inizializzazione principale del programma.
void init() {
	GLenum glErr;

	// Inizializzazione serpente
	snakeInit();

	// Seed per coordinate frutto
	srand(time(0));

	// Sfondo
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	// Depth buffer
	glEnable(GL_DEPTH_TEST);

	initVao();

	// Proiezione ortogonale
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0,5.0,-5.0,5.0,-2.0,2.0);

	// Controllo errori
    if ((glErr=glGetError()) != 0) {
        fprintf(stderr, "Errore = %d\n", glErr);
        //exit(-1);
		fprintf(stderr, "%s\n", gluErrorString(glErr));
    }

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Inizializzazione della lista collegata del serpente.
void snakeInit() {
	// Inizializzazione lista vuota
	head 		= NULL;
	lastBlock 	= NULL;

	// Inserimento di un blocco in coda
	for(int i  = 0; i < INITIAL_BLOCK_NO; i++) {
		appendBlock(initialBlocks[i]);
		
		// Salvo la posizione della testa
		if(i == 0)
			head = lastBlock;
	}
	
	// Inizializzazione direzione iniziale
	userDirection = right;
}

// Aggiunta di un blocco al corpo del serpente.
void appendBlock(int *newCoordinates) {
	// Allocazione nuovo elemento
	Snake *aux = malloc(sizeof(Snake));
	
	// Copia dei dati
	aux->block.coords[0] = newCoordinates[0];
	aux->block.coords[1] = newCoordinates[1]; 

	// Inizializzazione del trigger per collisione
	aux->block.trigger = 0;
	
	// Imposto il nuovo elemento come ultimo
	aux->next = NULL;

	// Se la lista è vuota, imposto questo elemento come primo
	if(lastBlock != NULL)
		lastBlock->next = aux;
	lastBlock = aux;  
}

// VAO.
void initVao() {
	// Generazione di due buffer ID
	glGenVertexArrays(2, vao);

	// vao[0]: cubi
	glBindVertexArray(vao[0]);

	// VBO: abilitazione buffer per vertici e colori
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glGenBuffers(2, cubeBuffers);
	// cubeBuffers[0]: vertici e colori
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexArray) + sizeof(cubeColorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertexArray), cubeVertexArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexArray), sizeof(cubeColorArray), cubeColorArray);

	// cubeBuffers[1]: indici
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeBuffers[1]);
	for(int i = 0; i < VERTEX_NO; i++)
		cubeVertexIndices[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertexIndices), cubeVertexIndices, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(cubeVertexArray)));

	// vao[1]: frutti
	glBindVertexArray(vao[1]);
	glGenBuffers(2, fruitBuffers);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	// fruitBuffers[0]: vertici e colori
	glBindBuffer(GL_ARRAY_BUFFER, fruitBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fruitVertexArray) + sizeof(fruitColorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fruitVertexArray), fruitVertexArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(fruitVertexArray), sizeof(fruitColorArray), fruitColorArray);

	// fruitBuffers[1]: indici
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fruitBuffers[1]);
	for(int i = 0; i < VERTEX_NO; i++)
		fruitVertexIndices[i] = i;
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fruitVertexIndices), fruitVertexIndices, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)sizeof(fruitVertexArray));
	/* buildVbo(cubeVertexArray, cubeColorArray, cubeVertexIndices, cubeBuffers, 0);
	buildVbo(fruitVertexArray, fruitColorArray, fruitVertexIndices, fruitBuffers, 1); */
}

/* void buildVbo(GLfloat *vertexArray, GLfloat *colorArray, unsigned int *vertexIndices, unsigned int *buffer, int vaoIndex) {
	// vao[0]: cubi
	glBindVertexArray(vao[vaoIndex]);

	// VBO: abilitazione buffer per vertici e colori
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glGenBuffers(2, &buffer);
	// cubeBuffers[0]: vertici e colori
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(&vertexArray) + sizeof(&colorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertexArray), &vertexArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(&vertexArray), sizeof(&colorArray), &colorArray);

	// cubeBuffers[1]: indici
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	for(int i = 0; i < VERTEX_NO; i++)
		vertexIndices[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(&vertexIndices), &vertexIndices, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(&vertexArray)));
} */

// Funzione per glutDisplayFunc.
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Input (movimento)
	processInput();
	
	// Se il frutto è stato mangiato, ne creo un altro
	if(fruit.trigger)
		newFruit();

	// Serpente
	drawSnakeHelper();

	// Frutto
	drawFruitHelper();
	
	// movate
	glFlush();
	glFinish();

	// Double buffering
	glutSwapBuffers();

	// Animazione del serpente
	glutTimerFunc(5000/60, redisplayHelper, 0);
}

// Modifica la scena in base all'input dell'utente.
void processInput() {
	// Avanza la testa del serpente
	updateSnakeHead();

	// Se i blocchi collidono, allora il serpente muore e il gioco finisce
	if(isDying()) {
		printf("Er pitonazzo è schiattato.\ngit gud\n");
		killSnake();
	}
	
	// Rilevamento collisione con il frutto: se la testa collide allora viene aggiunto un blocco
	if(detectCollision(fruit.coords, head->block.coords)) {
		fruit.trigger = 1;
		appendBlock(fruit.coords);
	}
}

// Aggiornamento della posizione della testa del serpente.
void updateSnakeHead() {
	/* Aggiornamento della posizione del serpente in base alle coordinate specificate
	 * dall'utente; gestione "effetto pacman"
	 */
	switch(userDirection) {
		case up:
			if( head->block.coords[1] < 10) { 
				yMov = 1;
                xMov = 0;
 			} else {
				yMov = -20;
                xMov = 0;
			}
			break;
		case down:    
			if( head->block.coords[1] > -10) {
				yMov = -1;
                xMov = 0;				
 			} else {
				yMov = 20;
				xMov =  0;
			}
			break;
		case left:
 			if( head->block.coords[0] > -10) {
				xMov = -1;
                yMov = 0;
				
 			} else {
				xMov = 20;
				yMov = 0;
			}
			break;
		case right:
 			if( head->block.coords[0] < 10) {
				xMov = 1;
                yMov = 0;
 			} else {
				xMov = -20;
                yMov = 0;
			}
			break;
		default:
			break;
	}

	// Salvo la posizione originale della testa
	for(int i = 0; i < 2; i++)
		ghostAuxCoords[i] = head->block.coords[i];
	
	// Aggiorno la posizione della testa
    head->block.coords[0] += xMov;
    head->block.coords[1] += yMov;
}

// Rilevamento morte del serpente.
int isDying() {
	// Rilevamento collisione tra blocchi: se almeno due vertici sono in comune, i blocchi collidono e il serpente muore
	for(Snake *death = head->next; death != NULL; death = death->next) {
		if(detectCollision(head->block.coords, death->block.coords))
			return 1;
	}
	return 0;
}

// Deallocazione dei blocchi del serpente e uscita dal programma.
void killSnake() {
	Snake *ptr = head;
	// Deallocazione di tutti i blocchi del serpente
	while(ptr != NULL) {
		Snake* aux = ptr;
		ptr = ptr->next;
		free(aux);
	}
	// Uscita
	exit(0);
}

/* Rilevamento collisione tra blocchi.
 * Confronta le coordinate dei due blocchi: se coincidono
 * allora sono sulla stessa cella e collidono.
 */
int detectCollision(int *block1, int *block2) {
	return block1[0] == block2[0] && block1[1] == block2[1];
}

// Calcolo della posizione del frutto.
void newFruit(void) {
	for(int i = 0; i < 2; i++)
		fruit.coords[i] = (int) rand() % 5;
	fruit.trigger = 0;
}

// Preparazione al disegno del serpente nella scena.
void drawSnakeHelper() {
	// Specifico il rispettivo Vertex Array
	glBindVertexArray(vao[0]);

	for(Snake *ptr = head; ptr != NULL; ptr = ptr->next) {
		if(ptr != head) {
			// Corpo
			/* Per far sì che il serpente si muova, è necessario che
			 * l'i-esimo blocco segua la posizione di quello precedente:
			 * si effettua quindi uno scambio di valori
			 * tramite una variabile ausiliaria.
			 */
			for(int i = 0; i < 2; i++) {
				snakeAuxCoords[i] 	 = ptr->block.coords[i];
				ptr->block.coords[i] = ghostAuxCoords[i];
				ghostAuxCoords[i] 	 = snakeAuxCoords[i];
			}
		}
		// Disegno a video
		drawElement(ptr->block.coords);
	}
}

// Preparazione al disegno del frutto nella scena.
void drawFruitHelper() {
	// Specifico l'array object per i vertici
	glBindVertexArray(vao[1]);
	// Disegno a video
	drawElement(fruit.coords);
}

// Disegna elementi a video.
void drawElement(int *translate) {
	glPushMatrix();
	// Utilizzo l'offset specificato in coordinate x,y
	glTranslatef(translate[0]*CELL, translate[1]*CELL, 0);
	// Prendo i vertici da disegnare dai VBO
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)0);
	glPopMatrix();
}

// Input da tastiera.
void keyInput(int key, int x, int y){
	switch(key) {
		case GLUT_KEY_UP:
			if(userDirection == down) 
				break;
			userDirection = up;
			break;
		case GLUT_KEY_DOWN:
			if(userDirection == up) 
				break;
			userDirection = down;
			break;
		case GLUT_KEY_RIGHT:
			if(userDirection == left) 
				break;
			userDirection = right;
			break;
		case GLUT_KEY_LEFT:
			if(userDirection == right) 
				break;
			userDirection = left;
			break;
		default:
			break;
	}
}

// Gestione tasti per uscita.
void exitHandler(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	// Esc
		case 'q':	// q
			// rilascio risorse e uscita
			killSnake();
		default:
			break;
	}
}

// Funzione helper per chiamata da glutTimerFunc.
void redisplayHelper(int val) {
	glutPostRedisplay();
}
