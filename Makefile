
CC := clang++
CCANALYZE := clang++ --analyze

INC := -I include
LIB :=
CFLAGS :=

SRCDIR := src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

BUILDDIR := build
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

TARGET := bin/renju

all:
	$(CC) $(CFLAGS) $(INC) $(LIB) -o $(TARGET) $(SOURCES)

analyze:
	$(CCANALYZE) $(CFLAGS) $(INC) $(LIB) $(SOURCES)
