CXX := g++
CXXFLAGS := -O3 -std=c++20 -Wno-interference-size
CXXDEBUGFLAGS := -g -std=c++20
BFQUEUE := bfqueue
RIGTORP_DIR := SPSCQueue
RIGTORP_BENCH := $(RIGTORP_DIR)/spsc_bench
DEBUG_BFQUEUE := debug_bfqueue
DEBUG_RIGTORP := debug_rigtorp

.PHONY: all run clean

all: $(BFQUEUE) $(RIGTORP_BENCH)

$(RIGTORP_DIR):
	git clone https://github.com/rigtorp/SPSCQueue.git $(RIGTORP_DIR)

$(RIGTORP_BENCH): $(RIGTORP_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(RIGTORP_DIR)/src/SPSCQueueBenchmark.cpp \
		-I $(RIGTORP_DIR)/include \
		-lpthread

$(BFQUEUE): benchmark.cpp BFQueue.h
	$(CXX) $(CXXFLAGS) -o $@ $< -lpthread

$(DEBUG_BFQUEUE): benchmark.cpp BFQueue.h
	$(CXX) $(CXXDEBUGFLAGS) -o $@ $< -lpthread

$(DEBUG_RIGTORP): $(RIGTORP_DIR)/src/SPSCQueueBenchmark.cpp
	$(CXX) $(CXXDEBUGFLAGS) -o $@ $< \
		-I $(RIGTORP_DIR)/include \
		-lpthread

run: all
	./$(RIGTORP_BENCH)
	./$(BFQUEUE)

debug: $(DEBUG_BFQUEUE) $(DEBUG_RIGTORP)

clean:
	rm -f $(BFQUEUE) $(DEBUG_BFQUEUE) $(RIGTORP_BENCH) $(DEBUG_RIGTORP)
