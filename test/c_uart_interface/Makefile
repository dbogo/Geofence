NAME = mavlink_control
CC = gcc

CFLAGS = -std=gnu11 -g -Wall -Wextra
DEPS_OPTS = -MM

INCLUDE = -Iinc/ -Imavlink/ -I.
# MAVLIB = mavlink/include/mavlink/v1.0/

SRC 	= $(foreach d, src/, $(wildcard $(d)*.c))
OBJS 	= $(patsubst %.c, %.o, $(SRC))
DEPS	= $(patsubst %.c, %.d, $(SRC))


%.d: %.c
	$(CC) $(DEPS_OPTS) $< -MT "$*.o $*.d" -MF $*.d

all: $(DEPS) $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) 

# Include dependencies (if there are any)
ifneq "$(strip $(DEPS))" ""
  include $(DEPS)
endif

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

.PHONY: graph
graph: $(NAME)
	mkdir Graphs
	mv *.expand Graphs/
	egypt Graphs/*.expand | dot -Tsvg -Grankdir=LR -o Graphs/$(NAME).svg
	eog Graphs/$(NAME).svg

.PHONY: clean
clean:
	rm -rf src/*.o src/*d $(NAME) Graphs/ *.expand
