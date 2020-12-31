# This is a Makefile for HW4: "Game of life"
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
CCLINK = $(CXX)
LIBS =
OBJS = main.o apple.o drawable.o drawable_list.o monster.o mini_gui.o
RM = rm -f

# Creating the executable
prog: $(OBJS)
	$(CCLINK) -o game.exe $(OBJS) $(LIBS)

# Creating object files using default rules
apple.o: apple.cpp apple.h drawable.h mini_gui.h ascii_objects.h
drawable.o: drawable.cpp drawable.h mini_gui.h ascii_objects.h
drawable_list.o: drawable_list.cpp drawable_list.h drawable.h mini_gui.h \
 ascii_objects.h
main.o: main.cpp drawable_list.h drawable.h mini_gui.h ascii_objects.h \
 apple.h monster.h
monster.o: monster.cpp monster.h drawable.h mini_gui.h ascii_objects.h \
 drawable_list.h apple.h
mini_gui.o: mini_gui.cpp mini_gui.h

# Cleaning old files before new make
clean:
	$(RM) *.exe *.o 
