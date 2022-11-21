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

}  // namespace bench

static void SoftHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    // state.PauseTiming();
    const auto rand = bench::generate_rand(state.range(0));
    // state.ResumeTiming();
    benchmark::DoNotOptimize(
        SoftHeap<int>(rand.begin(), rand.end(), 1.0 / state.range(1)));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void STLHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    // state.PauseTiming();
    const auto rand = bench::generate_rand(state.range(0));
    // state.ResumeTiming();
    benchmark::DoNotOptimize(
        std::priority_queue<int, std::vector<int>>(rand.begin(), rand.end()));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void SoftHeapInsert(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    const auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int>(rand.begin(), std::next(rand.begin(), state.range(0)),
                      1.0 / state.range(1));
    state.ResumeTiming();
    soft_heap.Insert(bench::generate_rand(1)[0]);
  }
  state.SetComplexityN(state.range(0));
}

static void STLHeapInsert(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    const auto rand = bench::generate_rand(state.range(0));
    auto min_queue =
        std::priority_queue<int, std::vector<int>>(rand.begin(), rand.end());
    state.ResumeTiming();
    min_queue.push(bench::generate_rand(1)[0]);
  }
  state.SetComplexityN(state.range(0));
}

static void SoftHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    const auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int>(rand.begin(), rand.end(), 1.0 / state.range(1));
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
    const auto rand = bench::generate_rand(state.range(0));
    auto min_queue =
        std::priority_queue<int, std::deque<int>>(rand.begin(), rand.end());
    state.ResumeTiming();
    // for (int i = 0; i < state.range(0); ++i) {
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(min_queue.top());
      min_queue.pop();
      benchmark::ClobberMemory();
    }
    // }
  }
  state.SetComplexityN(state.range(0));
}

static void VectorSortExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    std::sort(rand.begin(), rand.end(), std::less<>());
    benchmark::DoNotOptimize(rand.back());
    rand.erase(rand.begin());
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}
// BENCHMARK(SoftHeapConstruct)
//     ->ArgsProduct({{2 << 16}, {1, 1000}})
//     // ->ArgsProduct({{1, 8, 64, 128, 512, 1024}, {1, 1000}})
//     ->Complexity(benchmark::oLogN);
// BENCHMARK(STLHeapConstruct)
//     ->ArgsProduct({{2 << 16}})
//     // ->ArgsProduct({{1, 8, 64, 128, 512, 1024}, {1, 1000}})
//     ->Complexity(benchmark::oLogN);
// BENCHMARK(SoftHeapInsert)
//     ->ArgsProduct({{1, 8, 64, 128, 512, 1024, 2048}, {1, 10000}});
BENCHMARK(SoftHeapExtract)
    // ->ArgsProduct({{2 << 16}, {1, 1000}})
    ->ArgsProduct({{1, 8, 64, 128, 512, 1024}, {32, 8, 4, 2}});
// ->ArgsProduct({{1, 8, 64, 128, 512, 1024, 2048}, {1, 1000}});
// BENCHMARK(STLHeapInsert)->ArgsProduct({{1, 8, 64, 128, 512, 1024, 2048}});
BENCHMARK(STLHeapExtract)
    // ->ArgsProduct({{2 << 16}})
    ->ArgsProduct({{1, 8, 64, 128, 512, 1024}});

// BENCHMARK(VectorSortExtract)
//     ->ArgsProduct({{2 << 16}});
//     // ->ArgsProduct({{1, 8, 64, 128, 512, 1024}})

BENCHMARK_MAIN();

}  // namespace soft_heap
