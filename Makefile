CFLAGS = -g -Wall -Wextra -std=c11
OBJS = $(patsubst %.c, %.o, $(sort $(wildcard src/*.c)))
TARGET = kk

all: ${TARGET}

kk: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^

tags: $(wildcard *.c) $(wildcard *.h)
	@ctags --c-kinds=+lx *.c *.h


.PHONY: clean memcheck
clean:
	@rm -f ${OBJS} ${TARGET}

memcheck: ${TARGET}
	valgrind --tool=memcheck --leak-check=full --leak-check=full --show-leak-kinds=all ./${TARGET}
