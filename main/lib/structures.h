// Numero di vertici.
#define VERTEX_NO   4
// Numero di blocchi iniziali.
#define INITIAL_BLOCK_NO    6
// Numero di celle della griglia.
#define N_CELL  400
// Dimensione della singola cella.
#define CELL (1.0/2.0)

typedef struct Material {
    float	matAmbient[4],
			matDiffuse[4],
			matSpecular[4],
			shine[1];
} material_t;
/* struct _cube_t:
 * contiene informazioni sul singolo cubo.
 */
struct _cube_t {
    //GLfloat vertex[VERTEX_NO][2];   // Vertici (x,y)
    short int trigger;           	// Trigger in caso di collisione con altri cubi
    int coords[2];					// Coordinate x, y per la griglia virtuale
	/* int x,		
		y;  */						//each cube has a y-coordinate and x-coordinate inside of the matrix
    material_t material; 
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
