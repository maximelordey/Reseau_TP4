OBJ1 = fun.o
EXEC = ${OBJ1} transfertcl transfertse
all: ${EXEC}


fun.o :  fun.h fun.c
		gcc -DDEBUG -c fun.c -Wall

transfertcl.o : transfertcl.c
	gcc -c transfertcl.c -lcurses -Wall

transfertse.o : transfertse.c
	gcc -c transfertse.c -lcurses -Wall

transfertcl : transfertcl.o
	gcc -o transfertcl transfertcl.o ${OBJ1} 

transfertse : transfertse.o
	gcc -o transfertse transfertse.o ${OBJ1}


clean :
	rm -f ${EXEC} core
