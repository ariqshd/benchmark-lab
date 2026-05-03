## Set Build Mode
```
cmake --preset release
```

## Build project
```
cmake --build --preset release
```

## Run the benchmark
```
benchmark-lab\build\benchmarks\Release> .\bl_bench.exe
```
Trust CPU Time by Default: In most cases, we should ignore wall time and focus on CPU time for algorithmic analysis.


## Run the benchmark with repetition
```
benchmark-lab\build\benchmarks\Release> .\bl_bench.exe --benchmark_repetitions=10
```
The output will now show some additional statistical values. It includes the mean, and median, which are two different ways to calculate the average runtime across all repetitions.

It also includes the stddev (standard deviation), and cv (constant of variance), which are statistical measures of how consistent the results are across the repetitions.

Small values indicate our benchmarks reliably report similar results, whilst large values indicate a lot of variance and may indicate our benchmarks are somewhat unpredictable or particularly sensitive to those external effects.

## Automated Scaling with Range()
- Automatically rewriting test size by the given range
- Use `->Range()` modifier.
- Access the current input size using `state.range(0)`

```
BENCHMARK(BM_ListTraverse)->Range(8, 8 << 24);
```
The 8 << 24 syntax is a bit shift, which is a commonly used way of expressing large powers of two. 8 << 24 is equivalent to 8×2248×224

```
static void BM_ListTraverse(benchmark::State& state) {
  int64_t n = state.range(0);
}

BENCHMARK(BM_ListTraverse)->Range(8, 8 << 24);
```

## Determining Big O Complexity
- Tells the library to perform a regression analysis on the results
- It attempts to fit the data to known complexity curves such as O(n)O(n), O(n2)O(n2), O(log⁡n)O(logn), and tells us which one fits best.
- Use `->Complexity()` modifier
- Use `state.SetComplexityN()` to tell the library what value of n we're using

```C++
static void BM_ListTraverse(benchmark::State& state) {
  int64_t n = state.range(0);
  state.SetComplexityN(n);
}

  BENCHMARK(BM_ListTraverse)
    ->Range(8, 8 << 24)
    ->Complexity();

```

```
-----------------------------------------
Benchmark                             CPU
-----------------------------------------
BM_ListTraverse/8                 4.20 ns
BM_ListTraverse/64                53.0 ns
BM_ListTraverse/512                500 ns
BM_ListTraverse/4096              5625 ns
BM_ListTraverse/32768            71498 ns
BM_ListTraverse/262144          585938 ns
BM_ListTraverse/2097152       28125000 ns
BM_ListTraverse/16777216     223958333 ns
BM_ListTraverse/134217728   1812500000 ns
BM_ListTraverse_BigO             13.50 N
BM_ListTraverse_RMS                  1 %
```
The BM_ListTraverse_BigO value of 13.42N13.42N shows the library detected linear complexity, or O(n)O(n), as expected. The time to traverse our list scales linearly with the length of the list. 13.513.5 represents the scaling factor - each new item in our list increases the scan time by around 13.513.5 nanoseconds on this machine.

The RMS (root mean square) value is the error rate. We have a very low RMS at 1%, meaning the benchmark is very confident in this answer. A high RMS (e.g., 40%) means the data is erratic, and the big O classification might be wrong.
