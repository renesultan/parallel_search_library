# Analysis of N-Way Parallel Implementation Performance

## Overview

This analysis compares direct threading versus thread pool implementations across different thread counts and data sizes, with detailed examination of thread creation overhead and search time components.

## Performance Analysis by Data Size

### Small Dataset (10,000 elements)

#### Thread Creation Overhead

```
Threads | Original (μs) | Thread Pool (μs) | Overhead Difference
2       | 71           | 107              | +50.7% worse
4       | 87           | 93               | +6.9% worse
8       | 151          | 162              | +7.3% worse
16      | 309          | 370              | +19.7% worse
32      | 608          | 652              | +7.2% worse
```

#### Search Time

```
Threads | Original (μs) | Thread Pool (μs) | Performance Difference
2       | 72           | 117              | +62.5% worse
4       | 73           | 98               | +34.2% worse
8       | 59           | 145              | +145.8% worse
16      | 72           | 233              | +223.6% worse
32      | 129          | 425              | +229.5% worse
```

Key Findings:

- Original threading performs better for small datasets
- Thread pool overhead increases with thread count
- Significant search time penalty for thread pool with high thread counts

### Medium Dataset (100,000 elements)

#### Thread Creation Overhead

```
Threads | Original (μs) | Thread Pool (μs) | Overhead Difference
2       | 80           | 46               | -42.5% better
4       | 71           | 74               | +4.2% worse
8       | 144          | 156              | +8.3% worse
16      | 333          | 350              | +5.1% worse
32      | 595          | 626              | +5.2% worse
```

#### Search Time

```
Threads | Original (μs) | Thread Pool (μs) | Performance Difference
2       | 239          | 261              | +9.2% worse
4       | 144          | 172              | +19.4% worse
8       | 97           | 186              | +91.8% worse
16      | 85           | 267              | +214.1% worse
32      | 134          | 461              | +244.0% worse
```

Key Findings:

- Thread pool shows better thread creation time for 2 threads
- Original threading maintains better search performance
- Overhead differences become more pronounced with higher thread counts

### Large Dataset (1,000,000 elements)

#### Thread Creation Overhead

```
Threads | Original (μs) | Thread Pool (μs) | Overhead Difference
2       | 77           | 56               | -27.3% better
4       | 67           | 111              | +65.7% worse
8       | 119          | 151              | +26.9% worse
16      | 237          | 402              | +69.6% worse
32      | 869          | 1067             | +22.8% worse
```

#### Search Time

```
Threads | Original (μs) | Thread Pool (μs) | Performance Difference
2       | 2120         | 2025             | -4.5% better
4       | 1085         | 1133             | +4.4% worse
8       | 812          | 895              | +10.2% worse
16      | 828          | 1064             | +28.5% worse
32      | 267          | 607              | +127.3% worse
```

Key Findings:

- Thread pool shows advantages for low thread counts
- Search time differences become less pronounced
- Original threading still performs better at high thread counts

### Very Large Dataset (10,000,000 elements)

#### Thread Creation Overhead

```
Threads | Original (μs) | Thread Pool (μs) | Overhead Difference
2       | 62           | 94               | +51.6% worse
4       | 64           | 96               | +50.0% worse
8       | 194          | 163              | -16.0% better
16      | 261          | 475              | +82.0% worse
32      | 742          | 2937             | +295.8% worse
```

#### Search Time

```
Threads | Original (μs) | Thread Pool (μs) | Performance Difference
2       | 83279        | 28984            | -65.2% better
4       | 11037        | 11251            | +1.9% worse
8       | 9949         | 9977             | +0.3% worse
16      | 10352        | 9915             | -4.2% better
32      | 9248         | 108802           | +1076.5% worse
```

Key Findings:

- Thread pool shows significant advantage for 2 threads
- Performance becomes comparable at middle thread counts
- Severe degradation for thread pool at 32 threads

## Key Performance Patterns

### Thread Creation Overhead

1. **Small Datasets**

   - Original threading consistently better
   - Linear overhead increase with thread count
   - Thread pool overhead grows faster

2. **Large Datasets**

   - Thread pool better for low thread counts
   - Crossover point around 4-8 threads
   - Exponential overhead increase at high thread counts

### Search Time Scaling

1. **Thread Count Impact**

   - Optimal thread count: 4-8 for most cases
   - Diminishing returns after 8 threads
   - Performance degradation after 16 threads

2. **Implementation Differences**

   - Original threading more consistent
   - Thread pool better for specific cases
   - Higher variance in thread pool performance

## Recommendations

### For Small Datasets (<100K elements)

1. Use original threading implementation
2. Limit thread count to 4
3. Consider serial implementation

### For Medium Datasets (100K-1M elements)

1. Use original threading for >4 threads
2. Consider thread pool for 2-4 threads
3. Optimal thread count: 4-8

### For Large Datasets (>1M elements)

1. Use thread pool for 2-thread cases
2. Use original threading for >8 threads
3. Avoid 32+ threads regardless of implementation

## Implementation-Specific Guidelines

### Original Threading

- Best for consistent performance
- Better with high thread counts
- Lower overhead for small datasets

### Thread Pool

- Best for large datasets with few threads
- Better thread reuse characteristics
- Avoid high thread counts (>16)
