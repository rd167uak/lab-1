all: myshell.c
	gcc myshell.c -o myshell
run: myshell
	./myshell
