CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g \
		 -I/usr/local/include/igraph 
LDFLAGS = -L/usr/local/lib -ligraph 

OBJS = ./src/main.o

.PHONY: all exec check clean

all : exec

exec : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ./my-graph

clean:
	$(RM) $(OBJS) *~ my-graph src/*~