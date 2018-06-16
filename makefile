rubiksmake: cube.c movement.c 
	gcc -Wall -o cube cube.c movement.c solver.c -lGL -lGLU -lglut -lm
