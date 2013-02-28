#@ Author Kevin Harrington
SHELL := /bin/bash -e

CFILES := $(wildcard *.c)
OFILES := $(CFILES:%.c=%.o)

BIN :=BowlerImplimentationExample

ifndef CC_Bowler_Arch
    CC_Bowler_Arch :="gcc -Os -g3 -Wall -c -fmessage-length=0 -MMD -MP"
endif

all:clean $(OFILES)
	$(CC_Bowler_Arch) -L../../lib/native/linux -lNRLIB $(OFILES) -o $(BIN)
	chmod +x $(BIN)
%.o: %.c
	$(CC_Bowler_Arch) -I../../Platform/include/ -I../../BowlerStack/include/ -c $< -o $@
clean:
	 rm -rf *.o
	 rm -rf *.d
	 rm -rf $(BIN)