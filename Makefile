CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g \
		 -I/usr/local/include/igraph 
LDFLAGS = -L/usr/local/lib -ligraph 

OBJS = ./src/main.o

.PHONY: all exec check clean

all : exec

exec : $(OBJS)
	$(CC) -O3 src/diam.c -o ./diam 2> compilation-warnings
# 	$(CC) -O3 src/prelim.c -o ./prelim
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o ./my-graph

clean:
	$(RM) $(OBJS) my-graph diam prelim compilation-warnings
