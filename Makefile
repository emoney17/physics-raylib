all:
	clang -o physics physics.c -lraylib -lm -Wall -Wextra
clean:
	-rm physics
