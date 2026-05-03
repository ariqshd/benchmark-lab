#include <benchmark/benchmark.h>
#include <bl/vector.h>
#include <forward_list>
#include <random>

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
     int64_t n = state.range(0);
     state.SetComplexityN(n);

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

    state.SetItemsProcessed(state.iterations() * n); // show items per second
}
BENCHMARK(BM_VectorTraverse)
  ->Range(8, 8 << 24)
  ->Complexity();

static void BM_ListTraverse(benchmark::State& state) {
    int64_t n = state.range(0);
    state.SetComplexityN(n);

    // Setup: Create a large list
  std::forward_list<int> l(n);
  std::fill(l.begin(), l.end(), 1);

  for (auto _ : state) {
    long long sum = 0;

    // The Algorithm: Linear Scan
    for (int i : l) {
      sum += i;
    }

    benchmark::DoNotOptimize(sum);
  }

  state.SetItemsProcessed(state.iterations() * n); // show items per second
}
BENCHMARK(BM_ListTraverse)
    ->Range(8, 8 << 24)
    ->Complexity();

struct Player {
    int id;
    int score;
    int health;
};

std::vector<Player> GetSortedPlayers(int n){
    std::vector<Player> Result(n);
    for (int i = 0; i < n; ++i) Result[i].id = i * 2;
    return Result;
}

static void BM_LinearLookup(benchmark::State& state){
    int n = state.range(0);

    auto players = GetSortedPlayers(n);

    int target = n; // Look for middle-ish value

    for(auto _ : state) {
        auto result = std::ranges::find(players, target, &Player::id);
        benchmark::DoNotOptimize(result);
    }
}

static void BM_BinaryLookup(benchmark::State& state) {
    int n = state.range(0);
    auto players = GetSortedPlayers(n);
    int target = n;

   for(auto _ : state) {
       auto result = std::ranges::binary_search(players, target, {}, &Player::id);
       benchmark::DoNotOptimize(result);
   }
}

BENCHMARK(BM_LinearLookup)->RangeMultiplier(2)->Range(4, 128);
BENCHMARK(BM_BinaryLookup)->RangeMultiplier(2)->Range(4, 128);

std::vector<Player> GenerateRandomPlayers(int n) {
    std::vector<Player> result(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, n * 10);

    for(auto& p : result) {
        p.id = dist(rng);
    }

    return result;
}

void GenerateRandomQueries(std::vector<int>& queries, int max_id) {
  std::mt19937 rng(123); // Different seed
  std::uniform_int_distribution<int> dist(0, max_id);

  for (auto& q : queries) {
    q = dist(rng);
  }
}


const int N = 10000;

// No setup cost, high query cost
static void BM_RepeatedLinear(benchmark::State& state) {
    auto original_data = GenerateRandomPlayers(N);

    // Create K random queries
    int K = state.range(0);
    std::vector<int> queries(K);
    GenerateRandomQueries(queries, N * 10);

    for (auto _ : state) {
        std::vector<Player> players = original_data;

        // Perform K linear searches
        for (int target_id : queries) {
          auto result = std::ranges::find(
            players, target_id, &Player::id
          );
          benchmark::DoNotOptimize(result);
        }
    }
}

// High setup cost (Sort), low query cost
static void BM_SortAndBinary(benchmark::State& state) {
  auto original_data = GenerateRandomPlayers(N);

  // Create K random queries
  int K = state.range(0);
  std::vector<int> queries(K);
  GenerateRandomQueries(queries, N * 10);

  for (auto _ : state) {
      std::vector<Player> players = original_data;

      // THE COST: Sort the vector
      std::ranges::sort(players, {}, &Player::id);

      // THE BENEFIT: K binary searches
      for (int target_id : queries) {
        auto result = std::ranges::binary_search(
          players, target_id, {}, &Player::id
        );
        benchmark::DoNotOptimize(result);
      }
    }
}

BENCHMARK(BM_RepeatedLinear)
  ->Range(1, 1<<10) // Number of Queries (K)
  ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_SortAndBinary)
  ->Range(1, 1<<10) // Number of Queries (K)
  ->Unit(benchmark::kMillisecond);

// Since we linked benchmark::benchmark_main, we don't need
// to write our own main() function.
