CXX = gcc
GREEN_COLOR = `tput setaf 2`
BOLD = `tput bold`

SRC_CLI = ./src/client/sources/*.c
OBJ_CLI = $(SRC_CLI:.c=.o)
EXEC_CLI = ./build/client

SRC_SRV = ./src/server/sources/*.c
OBJ_SRV = $(SRC_SRV:.c=.o)
EXEC_SRV = ./build/server

CFLAGS = -g -Wall -Wextra `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` 
LFLAGS = -lconfig -pthread -DDEBUG -rdynamic

all:
	@$(CXX) $(OBJ_CLI) $(CFLAGS) $(LFLAGS) -o $(EXEC_CLI)
	@$(CXX) $(OBJ_SRV) $(CFLAGS) $(LFLAGS) -o $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Generated all executable files **"

client:
	@$(CXX) $(OBJ_CLI) $(CFLAGS) $(LFLAGS) -o $(EXEC_CLI)
	@echo "${GREEN_COLOR}${BOLD}** Generated client **"

server:
	@$(CXX) $(OBJ_SRV) $(CFLAGS) $(LFLAGS) -o $(EXEC_SRV) 
	@echo "${GREEN_COLOR}${BOLD}** Generated server **${GREEN_COLOR}${BOLD}"

.PHONY: clean
clean:
	@rm -rf $(EXEC_CLI) $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Removed all executable files **"

.PHONY: documentation
documentation:
	doxygen
	@echo "${GREEN_COLOR}${BOLD}** Generated documentation from doxygen **"