BINARY=bin
SOURCES=src
INCLUDES=include
LIBRARIES=lib

CC=g++
OPT=-O3
DEPFLAGS=-MP -MD
FLAGS=-Wall -Wextra -g $(foreach D,$(INCLUDES),-I$(D)) $(foreach D,$(LIBRARIES),-L$(D)) $(OPT) $(DEPFLAGS)

FILES=$(foreach D,$(SOURCES),$(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(FILES))
DEPENDENCIES=$(patsubst %.cpp,%.d,$(FILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(FLAGS) -c -o  $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPENDENCIES)
