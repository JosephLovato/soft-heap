#include <benchmark/benchmark.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "node.hpp"
#include "soft_heap.hpp"
#include "tree.hpp"

namespace soft_heap {

namespace bench {

[[nodiscard]] auto generate_rand(int n) noexcept {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
  std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
  return v;
}

[[nodiscard]] auto rand_int(int start, int stop) noexcept {
  auto generator = std::mt19937(std::random_device()());
  auto dist = std::uniform_int_distribution<int>(start, stop);
  return dist(generator);
}

}  // namespace bench

static void SoftHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        SoftHeap<int>(rand.begin(), rand.end(), 1.0 / state.range(1)));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void STLHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        std::priority_queue<int, std::vector<int>>(rand.begin(), rand.end()));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void SoftHeapInsert(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto rand_int = bench::rand_int(1, 100000);
    auto soft_heap =
        SoftHeap<int>(rand.begin(), std::next(rand.begin(), state.range(0)),
                      1.0 / state.range(1));
    state.ResumeTiming();
    // for (int i = 0; i < 1000; ++i) {
    soft_heap.Insert(rand_int);
    // }
  }
  state.SetComplexityN(state.range(0));
}

static void STLHeapInsert(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    const auto rand_int = bench::rand_int(1, 100000);
    auto min_queue = std::priority_queue<int>(rand.begin(), rand.end());
    state.ResumeTiming();
    benchmark::ClobberMemory();
    // for (int i = 0; i < 1000; ++i) {
    min_queue.push(rand_int);
    // }
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void SoftHeapExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int>(rand.begin(), rand.end(), 1.0 / state.range(1));
    state.ResumeTiming();
    benchmark::DoNotOptimize(soft_heap.ExtractMin());
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}
static void STLHeapExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto min_queue = std::priority_queue<int>(rand.begin(), rand.end());
    state.ResumeTiming();
    benchmark::DoNotOptimize(min_queue.top());
    benchmark::ClobberMemory();
    min_queue.pop();
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}
static void SoftHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap = SoftHeap<int, std::list<int>>(rand.begin(), rand.end(),
                                                   1.0 / state.range(1));
    state.ResumeTiming();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(soft_heap.ExtractMin());
      benchmark::ClobberMemory();
    }
  }
  state.SetComplexityN(state.range(0));
}
static void STLHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto min_queue = std::priority_queue<int>(rand.begin(), rand.end());
    state.ResumeTiming();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(min_queue.top());
      benchmark::ClobberMemory();
      min_queue.pop();
      benchmark::ClobberMemory();
    }
  }
  state.SetComplexityN(state.range(0));
}

static void VectorSortExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand_in = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    auto rand = std::vector(rand_in.begin(), rand_in.end());
    std::sort(rand.begin(), rand.end(), std::less<>());
    benchmark::DoNotOptimize(rand.back());
    benchmark::ClobberMemory();
    rand.erase(rand.begin());
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}
// BENCHMARK(SoftHeapConstruct)
//     // ->ArgsProduct({{2 << 16}, {1, 1000}})
//     ->ArgsProduct({{1, 8, 64, 128, 512, 1024}, {32, 8, 4, 2}});
// ->ArgsProduct({{1, 8, 64, 128, 512, 1024, 2048}, {1, 1000}});
// BENCHMARK(STLHeapInsert)
//     ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768}})
//     ->Complexity(benchmark::o1);
// BENCHMARK(SoftHeapExtract)
//     // ->ArgsProduct({{2 << 16}, {1, 1000}})
//     ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768},
//     {8}})
//     ->Complexity(benchmark::oNSquared);
// BENCHMARK(STLHeapExtract)
//     // ->ArgsProduct({{2 << 16}})
//     ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768}})
// ->Complexity(benchmark::oNLogN);
BENCHMARK(SoftHeapExtractOne)
    // ->ArgsProduct({{2 << 16}, {1, 1000}})
    // ->Threads(8)
    ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768},
                   {8, 16384}});
// ->Complexity(benchmark::oN);
BENCHMARK(STLHeapExtractOne)
    // ->ArgsProduct({{2 << 16}})
    // ->Threads(8)
    ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768}});
// ->Complexity(benchmark::oLogN);
BENCHMARK(VectorSortExtractOne)
    // ->ArgsProduct({{2 << 16}})
    ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768}});
BENCHMARK(STLHeapInsert)
    // ->ArgsProduct({{2 << 16}})
    ->ArgsProduct({{1, 8, 64, 128, 512, 1024}});
BENCHMARK(SoftHeapInsert)
    // ->ArgsProduct({{2 << 16}, {1, 1000}})
    ->ArgsProduct({{128, 512, 1024, 2048, 4096, 8192, 16384, 32768},
                   {8, 16384}});
// ->Complexity(benchmark::o1);
// BENCHMARK(STLHeapConstruct)
//     // ->ArgsProduct({{2 << 16}})
//     ->ArgsProduct({{1, 8, 64, 128, 512, 1024}});
// ->ArgsProduct({{1, 8, 64, 128, 512, 1024}})

BENCHMARK_MAIN();

}  // namespace soft_heap
