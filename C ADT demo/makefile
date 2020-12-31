# This is a Makefile for HW2: Elect_MMT
CC = gcc
CFLAGS = -g -Wall -std=c99
CCLINK = $(CC)
LIBS =
OBJS = main.o party.o voter.o
RM = rm -f

# Creating the executable
prog: $(OBJS)
	$(CCLINK) -o Elect_MMT $(OBJS) $(LIBS)

# Creating object files using default rules
main.o: main.c voter.h party.h
party.o: party.c party.h voter.h
voter.o: voter.c voter.h party.h

# Cleaning old files before new make
clean:
	$(RM) doom screen_test *.o *.bak *~ "#"* core
