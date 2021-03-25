CFLAGS = -g
OBJS = $(patsubst %.c, %.o, $(sort $(wildcard *.c)))
TARGET = kk

all: ${TARGET}

kk: ${OBJS}

.PHONY: clean
clean:
	@rm -f ${OBJS} ${TARGET}

tags: $(wildcard *.c) $(wildcard *.h)
	@ctags --c-kinds=+lx *.c *.h
