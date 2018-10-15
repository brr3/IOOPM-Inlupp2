COMPILER = gcc
OPTIONS = -g -Wall -pedantic
LINKOPTIONS = -lcunit

tests: elem.o logic.o ui.o tests.c
	$(COMPILER) $(OPTIONS) -o tests elem.o logic.o ui.o hash_table.o linked_list.o utils.o tests.c $(LINKOPTIONS)

elem.o: logic/elem.c logic/elem.h
	$(COMPILER) $(OPTIONS) -c logic/elem.c $(LINKOPTIONS)

logic.o: logic/logic.c logic/logic.h
	$(COMPILER) $(OPTIONS) -c logic/logic.c $(LINKOPTIONS)

ui.o: user_interface/ui.c user_interface/ui.h
	$(COMPILER) $(OPTIONS) -c user_interface/ui.c $(LINKOPTIONS)

dependencies: data_structures/linked_list.c data_structures/hash_table.c utils/utils.c 
	$(COMPILER) $(OPTIONS) -c data_structures/linked_list.c data_structures/hash_table.c utils/utils.c $(LINKOPTIONS)

clean:
	rm *.o

gitadd:
	git add *.c *.h *.md Makefile
