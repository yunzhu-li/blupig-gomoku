#
# renju-parallel
# Copyright (C) 2016 Yunzhu Li
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CX     ?= clang++
INC    := -I include
CFLAGS := -std=c++11

LIBDIR       := lib
SRCDIR       := src
SRCEXT       := cc
TESTSDIR     := tests
SOURCES      := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES_TEST := $(shell find $(TESTSDIR) -type f -name *.$(SRCEXT))

BINDIR     := bin
TARGET     := $(BINDIR)/renju

all:
	@mkdir -p $(BINDIR)
	@$(CX) $(CFLAGS) $(INC) -Ofast -o $(TARGET) $(SOURCES)

debug:
	@mkdir -p $(BINDIR)
	@$(CX) $(CFLAGS) $(INC) -g -o $(TARGET) $(SOURCES)

analyze:
	@mkdir -p $(BINDIR)
	@$(CX) --analyze $(CFLAGS) $(INC) $(SOURCES)

profile:
	@mkdir -p $(BINDIR)/prof
	@$(CX) -pg $(CFLAGS) $(INC) -o $(BINDIR)/a.out $(SOURCES)

test: build_gtest test_source

build_gtest:
	@mkdir -p $(LIBDIR)

	@# Compile gtest library
	@$(CX) -pthread -I 3rdparty -o $(LIBDIR)/gtest-all.o -c 3rdparty/gtest/gtest-all.cc
	@ar -r $(LIBDIR)/libgtest.a $(LIBDIR)/gtest-all.o
	@rm $(LIBDIR)/gtest-all.o

test_source:
	@mkdir -p $(BINDIR)

	@# Compile tests
	@$(CX) -pthread $(CFLAGS) $(INC) -I 3rdparty -D RENJU_PARALLEL_TEST \
	 -o $(TARGET)_test $(SOURCES_TEST) $(SOURCES) lib/libgtest.a

	@# Run tests
	@$(TARGET)_test
