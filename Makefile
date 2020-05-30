CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g \
		 -I/usr/local/include/igraph 
LDFLAGS = -L/usr/local/lib -ligraph 

OBJS = ./src/main.o ./src/clustering.o ./src/reorder.o ./src/utils.o

.PHONY: all exec check clean

all : exec

exec : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o ./vlg

clean:
	$(RM) $(OBJS) vlg
