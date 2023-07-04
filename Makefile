main:
	clang -o physics physics.c -lraylib -lm -Wall -Wextra
debug:
	clang -o physics physics.c -lraylib -lm -Wall -Wextra -DDEBUG
clean:
	-rm physics
