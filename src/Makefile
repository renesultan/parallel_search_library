CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
INC_DIR = -I./core -I./advanced/find_all_if -I./advanced/hybrid

# Test executables
TEST_TARGETS = original find_all_if find_all_if_future hybrid_advanced hybrid_early_termination n_way_parallel find_all_early_termination find_all_if_early_termination string_vector_test queue_based_test

all: $(TEST_TARGETS)

# Test executables
original: tests/main_original.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

find_all_if: tests/main_find_all_if.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

find_all_if_future: tests/main_find_all_if_future.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

hybrid_advanced: tests/main_hybrid_advanced.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

hybrid_early_termination: tests/main_hybrid_early_termination.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

n_way_parallel: tests/main_n_way_parallel.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

find_all_early_termination: tests/main_find_all_early_termination.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

find_all_if_early_termination: tests/main_find_all_if_early_termination.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

string_vector_test: tests/main_string_vector_test.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

queue_based_test: tests/main_queue_based_test.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@

clean:
	rm -f $(TEST_TARGETS)

.PHONY: all clean