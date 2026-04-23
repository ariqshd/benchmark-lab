#include <benchmark/benchmark.h>
#include <bl/vector.h>
#include <forward_list>

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

// The number of items we will insert
const int ITEM_COUNT = 10000;

static void BM_VectorInsertFront(benchmark::State& state){
    for(auto _ : state) {
        std::vector<int> v;

        for(int i = 0; i < ITEM_COUNT; ++i){
            // O(n) insertion - shifting elements
            v.insert(v.begin(), i);
        }

        // Prevent DCE
        benchmark::DoNotOptimize(v.data());
    }
}
BENCHMARK(BM_VectorInsertFront);

static void BM_ListInsertFront(benchmark::State& state) {
  for (auto _ : state) {
    std::forward_list<int> l;

    for (int i = 0; i < ITEM_COUNT; ++i) {
      // O(1) insertion - pointer update
      l.push_front(i);
    }

    // Prevent DCE - forward_list doesn't have .data()
    // so we use the front element
    benchmark::DoNotOptimize(l.front());
  }
}
BENCHMARK(BM_ListInsertFront);

static void BM_VectorTraverse(benchmark::State& state) {
  // Setup: Create a large vector
  std::vector<int> v(ITEM_COUNT);
  std::fill(v.begin(), v.end(), 1);

  for (auto _ : state) {
    long long sum = 0;

    // The Algorithm: Linear Scan
    for (int i : v) {
      sum += i;
    }

    benchmark::DoNotOptimize(sum);
  }
}
BENCHMARK(BM_VectorTraverse);

static void BM_ListTraverse(benchmark::State& state) {
  // Setup: Create a large list
  std::forward_list<int> l(ITEM_COUNT);
  std::fill(l.begin(), l.end(), 1);

  for (auto _ : state) {
    long long sum = 0;

    // The Algorithm: Linear Scan
    for (int i : l) {
      sum += i;
    }

    benchmark::DoNotOptimize(sum);
  }
}
BENCHMARK(BM_ListTraverse);



// Since we linked benchmark::benchmark_main, we don't need
// to write our own main() function.
