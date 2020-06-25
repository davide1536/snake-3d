CC := gcc
LDFLAGS := -lGL -lGLU -lglut -lGLEW -lm
WFLAGS := -Wall

all: snake.out

readBMP.o: main/readBMP.c
	$(CC) -o readBMP.o -c main/readBMP.c $(LDFLAGS) $(WFLAGS)

snake.o: main/snake.c
	$(CC) -o snake.o -c main/snake.c $(LDFLAGS) $(WFLAGS)

snake.out: snake.o readBMP.o
	$(CC) -o snake.out snake.o readBMP.o $(LDFLAGS) $(WFLAGS)

normals.o: main/snake.c
	$(CC) -o normals.o -c main/snake-normals.c $(LDFLAGS) $(WFLAGS)

snake-normal.out: normals.o readBMP.o
	$(CC) -o snake-normal.out normals.o readBMP.o $(LDFLAGS) $(WFLAGS)

clean:
	rm -f snake.*out snake.o readBMP.o