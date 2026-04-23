#include <benchmark/benchmark.h>
#include <bl/vector.h>

static void BM_MyVectorPush(benchmark::State& state) {
    bl::MyVector vec;

    // 2. The Measurement Loop
    for (auto _ : state) {
    // This code is timed
    vec.push_back(42);
    }
}

// 3. Register the function as a benchmark
BENCHMARK(BM_MyVectorPush)->Unit(benchmark::kNanosecond);;

// Since we linked benchmark::benchmark_main, we don't need
// to write our own main() function.
