EXEC = all-tests
CC = gcc

# includes and libs
INCS = -I. -I/usr/include `pkg-config --cflags glib-2.0`
LIBS = -L/usr/lib -lc -lev `pkg-config --libs glib-2.0`

# flags
CPPFLAGS = 
CFLAGS = -g -std=c99 -pedantic -Wall -O2 ${INCS}
LDFLAGS = ${LIBS}

PROG_SRC = ../../src/dict.c    \
		   ../../src/log.c     \
		   ../../src/node.c    \
		   ../../src/proto.c   \
		   ../../src/sandbox.c
SRC = $(wildcard *.c)
OBJ = ${SRC:.c=.o}

.PHONY: compile
compile: ${OBJ}
	@echo CC -o ${EXEC}
	@${CC} ${OBJ} ${PROG_SRC} ${LDFLAGS} -o ${EXEC}

.PHONY: run
run: compile
	@echo gtester ${EXEC}
	@gtester ${EXEC}

.PHONY: clean
clean:
	@echo cleaning
	@rm -f ${EXEC} *.o *.gch
