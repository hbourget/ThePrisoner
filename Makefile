CXX = gcc

SRC = ./sources/*.c
OBJ = $(SRC:.cc=.o)
EXEC = ./build/ThePrisoner_Server

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS) -lpthread

clean:
	rm -rf $(EXEC)

.PHONY: documentation
documentation:
	doxygen
