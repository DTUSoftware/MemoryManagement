CC = gcc
CCOPTS = -c -O2 -g -Wall -Wextra
LINKOPTS = -O2 -g -Wall -Wextra

EXEC=mem
OBJECTS=testrunner.o mymem.o memorytests.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LINKOPTS) -o  $@ $^

%.o:%.c
	$(CC) $(CCOPTS) -o $@ $^

clean:
	- $(RM) $(EXEC)
	- $(RM) $(OBJECTS)
	- $(RM) *~
	- $(RM) core.*

test: mem
	./$(EXEC) -test -f0 all all

stage1-test: mem
	./$(EXEC) -test -f0 all first

pretty: 
	indent *.c *.h -kr
