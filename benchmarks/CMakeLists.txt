# 1. Include the FetchContent module
include(FetchContent)

# 2. Configure Google Benchmark to skip its own tests and docs.
# This speeds up our build significantly.
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

# 3. Declare the external dependency
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(bl_bench main.cpp)

target_link_libraries(bl_bench PRIVATE
  benchmark::benchmark

  # Provides a default main() function for us
  benchmark::benchmark_main

  bl_core
)
