CXX := g++
CXXFLAGS := -O3 -std=c++20

CXXDEBUGFLAGS := -g -std=c++20

BFQUEUE	:= bfqueue
RIGTORP := rigtorp
DEBUG_BFQUEUE := debug_bfqueue

.PHONY: all run clean

all: $(BFQUEUE)

something: $(DEBUG_BFQUEUE)

$(DEBUG_BFQUEUE): benchmark.cpp
	$(CXX) $(CXXDEBUGFLAGS) -o $@ $<

$(BFQUEUE): benchmark.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: all
	./$(RIGTORP)
	./$(BFQUEUE)

debug: something

clean:
	rm -f $(BFQUEUE) $(DEBUG_BFQUEUE)

