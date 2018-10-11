COMPILER = gcc
OPTIONS = -g -Wall -pedantic
LINKOPTIONS = -lcunit

test_logic: logic.o logic_tests.c
	$(COMPILER) $(OPTIONS) logic.o /logic/logic_tests.c -o /logic/test_logic $(LINKOPTIONS)

logic.o: logic.c
	$(COMPILER) $(OPTIONS) -c /logic/logic.c $(LINKOPTIONS)

