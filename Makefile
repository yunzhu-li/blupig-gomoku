
CC        := clang++
CCANALYZE := clang++ --analyze
CCPROF    := g++ -pg

INC    := -I include
LIB    :=
CFLAGS := -std=c++11

SRCDIR  := src
SRCEXT  := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

BINDIR     := bin
PROFDIR    := bin/prof
TARGET     := $(BINDIR)/renju
PROFTARGET := $(PROFDIR)/a.out

all:
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(INC) $(LIB) -Ofast -o $(TARGET) $(SOURCES)

debug:
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(INC) $(LIB) -g -o $(TARGET) $(SOURCES)

analyze:
	@mkdir -p $(BINDIR)
	@$(CCANALYZE) $(CFLAGS) $(INC) $(LIB) $(SOURCES)

profile:
	@mkdir -p $(PROFDIR)
	@$(CCPROF) $(CFLAGS) $(INC) $(LIB) -o $(PROFTARGET) $(SOURCES)

