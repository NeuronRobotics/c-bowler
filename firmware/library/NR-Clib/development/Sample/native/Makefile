#@ Author Kevin Harrington
SHELL := /bin/bash -e

CFILES := $(wildcard *.c)
OFILES := $(CFILES:%.c=%.o)

BIN :=BowlerImplimentationExample

ifndef CC_Bowler_Arch
    CC_Bowler_Arch :=gcc -Os -g3 -Wall -fmessage-length=0 -MMD -MP
endif

all:clean $(OFILES)
	$(CC_Bowler_Arch) $(OFILES) -L../../lib/native/linux -lNRLIB_debug -o $(BIN)
	chmod +x $(BIN)
%.o: %.c
	$(CC_Bowler_Arch) -I../../Platform/include/ -I../../BowlerStack/include/ -c $< -o $@
clean:
	 rm -rf $(OFILES)