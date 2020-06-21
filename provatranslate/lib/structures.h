// Numero di vertici.
#define VERTEX_NO   4
// Numero di blocchi iniziali.
#define INITIAL_BLOCK_NO    6
// Velocità di movimento. (???)
#define N_CELL  400
#define CELL (1.0/2.0)

/* struct _cube_t:
 * contiene informazioni sul singolo cubo.
 */
struct _cube_t {
    GLfloat vertex[VERTEX_NO][2];   // Vertici (x,y)
    short int trigger;           // Trigger in caso di collisione con altri cubi
    int coordx;
    int coordy;                 //each cube has a y-coordinate and x-coordinate inside of the matrix
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
typedef enum {up, down, left, right, none} Input;
