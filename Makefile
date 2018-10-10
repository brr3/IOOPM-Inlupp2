COMPILER = gcc
OPTIONS = -g -Wall -pedantic
LINKOPTIONS = -lcunit -pg

test_logic: logic.o test_logic
	$(COMPILER) $(OPTIONS) logic.o logic_tests.c -o test_logic $(LINKOPTIONS)

logic.o: logic.h logic.c
  $(COMPILER) $(OPTIONS) -c logic.c logic.h $(LINKOPTIONS)
