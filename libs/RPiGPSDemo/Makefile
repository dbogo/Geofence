CC = gcc

LIBDIR 	= dist/Debug/
LIBNAME = libRPiGPSDemo.so
src = $(wildcard src/*.c)
obj = $(src:.c=.o)

CFLAGS 	= -fPIC -Wall -Wextra -g -c -I../../ -I../log4c/include/
LDFLAGS = -shared -L../log4c/lib -llog4c -lm

.PHONY: all
all: ${LIBNAME}

$(LIBNAME): $(obj)
	$(CC) $(LDFLAGS) -o $@ $^

$(src:.c=.d):%.d:%.c
	$(CC) $CFLAGS -MM $< >$@

.PHONY: clean
clean:
	rm -f $(src:.c=.d) $(obj) $(LIBNAME)