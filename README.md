# Parallel Find-All Library

A high-performance C++11 header-only library implementing parallel search algorithms for finding all occurrences of elements in containers. Includes multiple implementation strategies optimized for different use cases.

## Features

- Template-based design supporting any container and value type
- Multiple implementation strategies:
  - Basic serial implementation
  - Direct parallel implementation with configurable thread count
  - Future-based parallel implementation
  - Queue-based implementation with dynamic work distribution
  - Early termination variants for existence checking
  - Hybrid implementations that auto-select optimal strategy
- Comprehensive performance analysis and testing suite

## Requirements

- C++11 compatible compiler
- pthread support
- Make build system

## Project Structure

```
src/
├── Makefile
├── Unified-Report.md
├── README.md
├── core/
│   ├── find_all.h
│   ├── parallel_find_all.h
│   ├── find_all_early_termination.h
│   ├── find_all_if_early_termination.h
│   ├── find_all_queue.h
│   └── thread_pool.h
├── advanced/
│   ├── find_all_if/
│   │   ├── find_all_if.h
│   │   └── find_all_if_future.h
│   └── hybrid/
│       ├── hybrid_find_all.h
│       └── hybrid_find_all_advanced.h
│       └── hybrid_find_all_early_termination.h
├── tests/
│   ├── main_original.cpp
│   ├── main_find_all_if.cpp
│   ├── main_find_all_if_future.cpp
│   ├── main_hybrid_advanced.cpp
│   ├── main_hybrid_early_termination.cpp
│   ├── main_n_way_parallel.cpp
│   ├── main_find_all_early_termination.cpp
│   ├── main_find_all_if_early_termination.cpp
│   ├── main_string_vector_test.cpp
│   └── main_queue_based_test.cpp
├── analysis/ (each implmentation has its specific and respective analysis report)
├── results/ (each technique has its specific results reported)


```

## Quick Start

1. Include the appropriate header:

```cpp
#include "core/find_all.h"            // For serial implementation
#include "core/parallel_find_all.h"   // For parallel implementation
```

2. Basic usage:

```cpp
std::vector<int> numbers = {1, 2, 3, 2, 4, 2};
int search_value = 2;

// Serial search
auto results = find_all(numbers, search_value);

// Parallel search with 4 threads
auto parallel_results = parallel_find_all(numbers, search_value, 4);
```

## Implementation Selection Guide

Choose the appropriate implementation based on your use case:

- **Small datasets (<20K elements)**: Use serial implementation

  ```cpp
  #include "core/find_all.h"
  auto results = find_all(container, value);
  ```

- **Medium datasets (20K-100K elements)**: Use future-based implementation

  ```cpp
  #include "advanced/find_all_if/find_all_if_future.h"
  auto results = parallel_find_all_if_future(container, value, 4);  // 4 threads
  ```

- **Large datasets (>100K elements)**: Use queue-based implementation

  ```cpp
  #include "core/find_all_queue.h"
  auto results = parallel_find_all_queue(container, value, 8);  // 8 threads
  ```

- **Existence checking**: Use early termination variant

  ```cpp
  #include "core/find_all_early_termination.h"
  auto results = parallel_find_all_et(container, value, 4);
  ```

- **Automatic optimization**: Use hybrid implementation

  ```cpp
  #include "advanced/hybrid/hybrid_find_all_advanced.h"
  auto results = hybrid_find_all_advanced(container, value);
  ```

## Thread Count Guidelines

- Small datasets (<100K): 2-4 threads
- Medium datasets (100K-1M): 4-8 threads
- Large datasets (>1M): 8-16 threads

## Building and Testing

```bash
make all          # Build all test executables
make original     # Build specific test
make clean        # Clean build files
```

Run tests:

```bash
./original
./find_all_if
./hybrid_advanced
```

## Performance Considerations

- Break-even point for parallelization is ~20,000 elements
- String operations benefit more from parallelization than integers
- Early termination provides 50-100x speedup for beginning-of-container matches
- Thread creation overhead is 150-200μs for direct threading
- Queue-based implementation performs best with uneven workloads

## Contact

René Sultan - rs4240@columbia.edu
