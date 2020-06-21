// Inizializzazione del serpente.
void snakeInit();
// Aggiunta in coda di un blocco del serpente.
void appendBlock();
// Deallocazione dei blocchi del serpente.
void killSnake();
// Gestione dell'input inserito dall'utente.
void processInput();
// Disegno del serpente sulla scena.
void drawSnake();
//Controllo se ho mangiato il frutto
void checkFruit();
//Controllo se mi sono scontrato con me stesso 
void checkDeath();
//inizializzo le luci
void initLight();
//write score
void writeScore();
void writeBitmapString(void *font, char *string);
