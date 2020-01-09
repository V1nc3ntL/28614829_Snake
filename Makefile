.PHONY=clean
CC=gcc
SOURCES=main.c snakeAPI.c clientAPI.c 
OBJ_LIST=$(SOURCES:.c=.o)
CFLAGS= -W -Wall

exec: $(OBJ_LIST)
	$(CC) $^ -o $@

debug: $(OBJ_LIST)
	$(CC) -g $^ -o $@
.depend:
	gcc -MM $(SOURCES) > $@

-include .depend

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean: 
	rm -f *.o ${OBJ_LIST}
