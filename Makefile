CXX = gcc

SRC = ./sources/*.c
OBJ = $(SRC:.cc=.o)
EXEC = ./build/ThePrisoner_Server

CFLAGS=-DDEBUG

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $(OBJ) $(LBLIBS) -lpthread

clean:
	rm -rf $(EXEC)

.PHONY: documentation
documentation:
	doxygen
