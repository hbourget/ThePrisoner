CXX = gcc
GREEN_COLOR=`tput setaf 2`
BOLD=`tput bold`

SRC_CLI = ./src/client/sources/*.c
OBJ_CLI = $(SRC_CLI:.cc=.o)
EXEC_CLI = client

SRC_SRV = ./src/server/sources/*.c
OBJ_SRV = $(SRC_SRV:.cc=.o)
EXEC_SRV = server

CFLAGS= -DDEBUG
LFLAGS= -lconfig -pthread

all: $(EXEC_CLI) $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Generated all executable files **"

client:
	$(CXX) $(CFLAGS) -o $@ $(OBJ_CLI) $(LBLIBS) $(LFLAGS)
	@echo "${GREEN_COLOR}${BOLD}** Generated client **"

server:
	$(CXX) $(CFLAGS) -o $@ $(OBJ_SRV) $(LBLIBS) $(LFLAGS)
	@echo "${GREEN_COLOR}${BOLD}** Generated server **"

.PHONY: clean
clean:
	rm -rf $(EXEC_CLI) $(EXEC_SRV)
	@echo "${GREEN_COLOR}${BOLD}** Removed all executable files **"

.PHONY: documentation
documentation:
	doxygen
	@echo "${GREEN_COLOR}${BOLD}** Generated documentation from doxygen **"