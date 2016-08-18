
CC := clang++
CCANALYZE := clang++ --analyze

INC := -I include
LIB :=
CFLAGS :=

SRCDIR := src
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

BINDIR := bin
TARGET := $(BINDIR)/renju

all:
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(INC) $(LIB) -o $(TARGET) $(SOURCES)

analyze:
	@mkdir -p $(BINDIR)
	@$(CCANALYZE) $(CFLAGS) $(INC) $(LIB) $(SOURCES)
