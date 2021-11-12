CXX = g++

SRC = ./sources/*.c
OBJ = $(SRC:.cc=.o)
EXEC = ./build/socketServer

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS) -fpermissive -lpthread

clean:
	rm -rf $(EXEC)

.PHONY: documentation
documentation:
	doxygen