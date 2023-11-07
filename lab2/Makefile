PROGNAME ?= main
SRCNAME = $(PROGNAME).cpp
BINNAME = $(PROGNAME)

CXX ?= g++

CFLAGS ?= -O2 -Wall -Wextra -pedantic
CFLAGS_DEBUG = -O0 -Wall -Wextra -pedantic -fsanitize=address -fsanitize=undefined
LDFLAGS_DEBUG = -g

build: debug

release: $(SRCNAME)
	$(CXX) $(CFLAGS) $(SRCNAME) -o $(BINNAME)

debug: $(SRCNAME)
	$(CXX) $(CFLAGS_DEBUG) $(SRCNAME) -o $(BINNAME) $(LDFLAGS_DEBUG)

clean: $(BINNAME)
	rm $(BINNAME)
