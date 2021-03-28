CFLAGS = -g -Wall -Wextra -std=c11
OBJS = $(patsubst %.c, %.o, $(sort $(wildcard *.c)))
TARGET = kk

all: ${TARGET}

kk: ${OBJS}

.PHONY: clean recompile
clean:
	@rm -f ${OBJS} ${TARGET}

recompile:
	make clean && make

tags: $(wildcard *.c) $(wildcard *.h)
	@ctags --c-kinds=+lx *.c *.h

memcheck: ${TARGET}
	@valgrind --tool=memcheck --leak-check=full --leak-check=full --show-leak-kinds=all ./${TARGET}
