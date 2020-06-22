/**** SERPENTE ****/
// Inizializzazione del serpente.
void snakeInit(void);
// Aggiunta in coda di un blocco del serpente.
void appendBlock(int*);
// Deallocazione dei blocchi del serpente e uscita dal programma.
void killSnake(void);
// Gestione dell'input inserito dall'utente.
void processInput(void);
// Preparazione al disegno del serpente nella scena.
void drawSnakeHelper(void);
// Aggiornamento della posizione del serpente.
void updateSnakeHead(void);
// Rilevamento collisione tra blocchi.
int detectCollision(int*, int*);
// Rilevamento morte del serpente.
int isDying(void);

/**** FRUTTO ****/
// Calcolo della posizione di un nuovo frutto.
void newFruit(void);
// Preparazione al disegno del frutto nella scena.
void drawFruitHelper(void);

/**** ALTRO ****/
// VAO.
void initVao(void);
/* // VBO.
void buildVbo(GLfloat*, GLfloat*, unsigned int*, unsigned int*, int); */
// Funzione helper per chiamata da glutTimerFunc.
void redisplayHelper(int);
// Gestione tasti per uscita.
void exitHandler(unsigned char, int, int);
// Input da tastiera.
void keyInput(int, int, int);
// Modifica la scena in base all'input dell'utente.
void processInput(void);
// Funzione per glutDisplayFunc.
void display(void);
// Inizializzazione principale del programma.
void init(void);
// Disegna elementi a video.
void drawElement(int*);