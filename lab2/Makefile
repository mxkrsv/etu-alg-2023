PROGNAME ?= main
SRCNAME = $(PROGNAME).cpp
BINNAME = $(PROGNAME)

CXX ?= g++

CFLAGS ?= -O2 -Wall -Wextra -pedantic
CFLAGS_DEBUG = -O0 -Wall -Wextra -pedantic -fsanitize=address -fsanitize=undefined -DDEBUG
LDFLAGS_DEBUG = -g

MASSIF_PEAK_USAGE = ./massif_peak_usage

BENCH_POWS ?= 215
BENCH_MSIZES ?= 16 32 64 128 256

build: debug

release: $(SRCNAME)
	$(CXX) $(CFLAGS) $(SRCNAME) -o $(BINNAME)

debug: $(SRCNAME)
	$(CXX) $(CFLAGS_DEBUG) $(SRCNAME) -o $(BINNAME) $(LDFLAGS_DEBUG)

clean: $(BINNAME)
	rm $(BINNAME)

massif_peak_usage:
	make -C$(MASSIF_PEAK_USAGE)

clean_massif_peak_usage:
	make -C$(MASSIF_PEAK_USAGE) clean

bench: release
	./bench.sh "$(BENCH_POWS)" "$(BENCH_MSIZES)"

.PHONY: massif_peak_usage clean_massif_peak_usage
