// Numero di vertici.
#define VERTEX_NO   4
//numero di facce di ogni cubo che costituisce lo snake
#define FACES_NO 5
// Numero di blocchi iniziali.
#define INITIAL_BLOCK_NO    6
// Valore del frutto.
#define FRUIT_SCORE 10

// Numero di celle della griglia.
#define N_CELL  400
// Dimensione della singola cella.
#define CELL (1.0/2.0)
#define N_COL 20
#define N_ROWS 20
// Numero di vertici da disegnare per la griglia.
#define MATRIX_VERTEX_NO 4
// Step della griglia.
#define STEP 0.25

/* struct _material_t:
 * contiene informazioni sui materiali degli elementi della scena.
 */
struct _material_t {
    float	matAmbient[4],
			matDiffuse[4],
			matSpecular[4],
			shine[1];
};
typedef struct _material_t Material;

/* struct _cube_t:
 * contiene informazioni sul singolo cubo.
 */
struct _cube_t {
    short int trigger;           	// Trigger in caso di collisione con altri cubi
    int coords[2];					// Coordinate x, y per la griglia virtuale
};
typedef struct _cube_t Cube;

/* struct _snake_t: 
 * lista collegata di cubi.
 * Una variabile Snake* list sarà quindi puntatore al primo elemento della lista
 */
typedef struct _snake_t {
    Cube block;
    struct _snake_t* next;
} snake_t;
typedef snake_t Snake;

// Definizione input per direzionare il serpente.
typedef enum {up, down, left, right} Input;
