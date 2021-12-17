CC = gcc
SH = bash
GREEN_COLOR=`tput setaf 2`
BOLD=`tput bold`

SRC_CLI = ./src/client/sources/*.c
OBJ_CLI = $(SRC_CLI:.cc=.o)
EXEC_CLI = ./build/Client

SRC_SRV = ./src/server/sources/*.c
OBJ_SRV = $(SRC_SRV:.cc=.o)
EXEC_SRV = ./build/Server

CFLAGS= -g -lconfig -pthread -DDEBUG -lm `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -rdynamic 

all: $(EXEC_CLI) $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Generated all executable files **"

$(EXEC_CLI):
	$(CC) $(CFLAGS) -o $@ $(OBJ_CLI)
	@echo "${GREEN_COLOR}${BOLD}** Generated client **"

$(EXEC_SRV):
	$(CC) $(CFLAGS) -o $@ $(OBJ_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Generated server **"

.PHONY: clean
clean:
	rm -rf $(EXEC_CLI) $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Removed all executable files **"

.PHONY: documentation
documentation:
	doxygen
	@echo "${GREEN_COLOR}${BOLD}** Generated documentation from doxygen **"