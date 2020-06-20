# Snake 3D

[TOC]

# Features

Gameplay:

-   avanzando di livello, il serpente si muove più velocemente
-   punto di vista intercambiabile (telecamera esterna/testa snake)

OpenGL:

-   3D
-   Lighting
-   Shading
-   Texture
-   Frustum
-   VBO
-   Menu
    -   cambio del colore del serpente
    -   reset partita
    -   pausa
-   Keyboard events

# Piano di progetto

1.  Scena
    -   mappa (griglia)
        -   effetto "pacman"
        -   frustum
        -   VBO
2.  Snake
    -   cubi, collisione
3.  Integrazione

# Appunti

## Snake

Lista dinamica.
Ogni quadrato è un'allocazione di memoria in più

```c
struct Snake {
    cube_list body;		// lista dinamica doppiamente collegata di cubi
    Node* head; 		// puntatore alla testa
} snake_t;
```

## Cubo

Ogni cubo segue quello precedente;
ogni cubo può chiamare un metodo che verifica se un cubo collide l'altro

```c
struct Cube {
	GLfloat vertexes[8];	// vertici per cubo 3D
	bool isTriggered;		// true se il cubo collide con un altro
} cube_t;
```

