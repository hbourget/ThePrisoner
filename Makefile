CXX = gcc

SRC_CLI = ./src/client/sources/*.c
OBJ_CLI = $(SRC_CLI:.cc=.o)
EXEC_CLI = ./build/client

SRC_SRV = ./src/server/sources/*.c
OBJ_SRV = $(SRC_SRV:.cc=.o)
EXEC_SRV = ./build/server

CFLAGS = -g `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` 
LFLAGS = -lconfig -pthread -DDEBUG -rdynamic

all:
	@$(CXX) $(OBJ_CLI) $(CFLAGS) $(LFLAGS) -o $(EXEC_CLI)
	@$(CXX) $(OBJ_SRV) $(CFLAGS) $(LFLAGS) -o $(EXEC_SRV)
	@echo "\033[92m** Generated all executable files **\033[0m"

client:
	@$(CXX) $(OBJ_CLI) $(CFLAGS) $(LFLAGS) -o $(EXEC_CLI)
	@echo "\033[92m** Generated client **\033[0m"

server:
	@$(CXX) $(OBJ_SRV) $(CFLAGS) $(LFLAGS) -o $(EXEC_SRV) 
	@echo "\033[92m** Generated server **\033[0m"

.PHONY: clean
clean:
	@rm -rf $(EXEC_CLI) $(EXEC_SRV)
	@echo "\033[33m** Removed all executable files **\033[0m"

.PHONY: documentation
documentation:
	doxygen
	@echo "\033[94m** Generated documentation from doxygen **\033[0m"