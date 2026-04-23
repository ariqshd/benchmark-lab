#include <benchmark/benchmark.h>
#include <bl/vector.h>

// Benchmark 1: Standard Vector Copy
// Measures allocation + copy cost
static void BM_VectorCopy(benchmark::State& state) {
  // Setup outside the loop
  std::vector<int> src(1000, 42);

  for (auto _ : state) {
    // The operation being timed
    std::vector<int> copy{src};

    // Prevent Dead Code Elimination
    // If we didn't do this, the compiler might delete
    // the copy entirely since 'copy' is never used.
    benchmark::DoNotOptimize(copy.data());
  }
}
BENCHMARK(BM_VectorCopy);

// Benchmark 2: Baseline Memcpy
// Measures pure memory bandwidth (L1 Cache)
static void BM_Memcpy(benchmark::State& state) {
  std::vector<int> src(1000, 42);
  std::vector<int> dst(1000);

  for (auto _ : state) {
    std::memcpy(
      dst.data(), src.data(), src.size() * sizeof(int)
    );

    // Prevent optimization
    benchmark::DoNotOptimize(dst.data());
  }
}
BENCHMARK(BM_Memcpy);

// Benchmark 3: Math with Barriers
// Measures the cost of an ADD instruction,
// ensuring inputs aren't constant-folded.
static void BM_MathAdd(benchmark::State& state) {
  int a = 10;
  int b = 20;

  for (auto _ : state) {
    // Prevent Constant Folding on Inputs
    // Force compiler to treat 'a' and 'b' as
    // unknown runtime values
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    int c = a + b;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(BM_MathAdd);

// Since we linked benchmark::benchmark_main, we don't need
// to write our own main() function.
