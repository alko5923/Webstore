.PHONY: assignment2

C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
OBJECTS = utils.o linked_list.o hash_table.o webstore_frontend.o webstore_backend.o 

%.o:	%.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

clean:
	rm -f *.o assignment2

assignment2: $(OBJECTS)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(OBJECTS) -o assignment2
	valgrind --leak-check=full ./assignment2

utils.o: utils.c utils.h
	gcc -c -g -Wall utils.c

linked_list.o: linked_list.c linked_list.h
	gcc -c -g -Wall linked_list.c
	
hash_table.o: hash_table.c hash_table.h linked_list.c linked_list.h
	gcc -c -g -Wall hash_table.c
	
webstore_frontend.o: webstore_frontend.c webstore_backend.c utils.c hash_table.c linked_list.c backend.h linked_list.h utils.h hash_table.h
	gcc -c -g -Wall webstore_frontend.c

webstore_backend.o: webstore_backend.c hash_table.c linked_list.c utils.c backend.h utils.h linked_list.h
	gcc -c -g -Wall webstore_backend.c

