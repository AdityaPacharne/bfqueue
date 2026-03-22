CXX := g++
CXXFLAGS := -O3 -std=c++20

BFQUEUE	:= bfqueue
RIGTORP := rigtorp

.PHONY: all run clean

all: $(BFQUEUE)

$(BFQUEUE): benchmark.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: all
	./$(RIGTORP)
	./$(BFQUEUE)

clean:
	rm -f $(BFQUEUE)
