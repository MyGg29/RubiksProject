rubiksmake: cube.c movement.c 
	gcc -o cube cube.c movement.c solver.c  -lGL -lGLU -lglut -lm -g3
