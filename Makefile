CXX = gcc

SRC = ./sources/*.c
OBJ = $(SRC:.cc=.o)
EXEC = ./build/ThePrisoner_Server

CFLAGS= -DDEBUG
LFLAGS= -lconfig -pthread

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $(OBJ) $(LBLIBS) $(LFLAGS)

clean:
	rm -rf $(EXEC)

.PHONY: documentation
documentation:
	doxygen
