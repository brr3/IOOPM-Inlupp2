COMPILER = gcc
OPTIONS = -g -Wall -pedantic
LINKOPTIONS = -lcunit

test_logic: elem.o logic.o ui.o logic/logic_tests.c
	$(COMPILER) $(OPTIONS) -o test_logic elem.o logic.o ui.o hash_table.o linked_list.o utils.o logic/logic_tests.c $(LINKOPTIONS)

elem.o: logic/elem.c
	$(COMPILER) $(OPTIONS) -c logic/elem.c $(LINKOPTIONS)

logic.o: logic/logic.c
	$(COMPILER) $(OPTIONS) -c logic/logic.c $(LINKOPTIONS)

ui.o: user_interface/ui.c
	$(COMPILER) $(OPTIONS) -c user_interface/ui.c $(LINKOPTIONS)

dependencies: data_structures/linked_list.c data_structures/hash_table.c utils/utils.c 
	$(COMPILER) $(OPTIONS) -c data_structures/linked_list.c data_structures/hash_table.c utils/utils.c $(LINKOPTIONS)

clean:
	rm *.o

gitadd:
	git add *.c *.h *.md Makefile
