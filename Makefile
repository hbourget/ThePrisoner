CXX = gcc

SRC = ./sources/*.c
OBJ = $(SRC:.cc=.o)
EXEC = ./build/ThePrisoner_Server
GREEN_COLOR=`tput setaf 2`
BOLD=`tput bold`

CFLAGS= -DDEBUG
LFLAGS= -lconfig -pthread

$(EXEC): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $(OBJ) $(LBLIBS) $(LFLAGS)

.PHONY: all
all: $(EXEC)
	@echo "${GREEN_COLOR}${BOLD}** Generated executable file **"

.PHONY: clean
clean:
	rm -rf $(EXEC)
	@echo "${GREEN_COLOR}${BOLD}** Removed executable file **"

.PHONY: documentation
documentation:
	doxygen
	@echo "${GREEN_COLOR}${BOLD}** Generated documentation from doxygen **"
