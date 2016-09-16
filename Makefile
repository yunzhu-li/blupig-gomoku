
CC := clang++
CCANALYZE := clang++ --analyze
CCPROF := g++ -pg

INC := -I include
LIB :=
CFLAGS := -std=c++11

SRCDIR := src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

BINDIR := bin
TARGET := $(BINDIR)/renju

all:
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(INC) $(LIB) -o $(TARGET) $(SOURCES)

debug:
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(INC) $(LIB) -g -o $(TARGET) $(SOURCES)

analyze:
	@mkdir -p $(BINDIR)
	@$(CCANALYZE) $(CFLAGS) $(INC) $(LIB) $(SOURCES)

profile:
	@mkdir -p $(BINDIR)
	@$(CCPROF) $(CFLAGS) $(INC) $(LIB) $(SOURCES)

