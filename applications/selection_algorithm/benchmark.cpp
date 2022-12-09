#include <benchmark/benchmark.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <vector>

#include "selection_algorithm.hpp"

using namespace selection_algorithm;

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

static void Nth_Element(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto k = rand.begin() + rand.size() / 2;
    state.ResumeTiming();
    std::nth_element(rand.begin(), k, rand.end());
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void standard_heap(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto min_heap = std::vector<int>(rand.begin(), rand.end());
    std::make_heap(min_heap.begin(), min_heap.end(), std::greater<>{});
    auto k = rand.size() / 2;
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        selection_algorithm::standard_heap_selection(min_heap, k));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

static void standard_heap_vector(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto min_heap = std::vector<int>(rand.begin(), rand.end());
    // std::make_heap(min_heap.begin(), min_heap.end(), std::greater<>{});
    auto k = rand.size() / 2;
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        selection_algorithm::standard_heap_selection_vector(min_heap, k));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

// static void standard_heap_iterator(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand = bench::generate_rand(state.range(0));
//     auto min_heap = std::vector<int>(rand.begin(), rand.end());
//     auto k = rand.size() / 2;
//     state.ResumeTiming();
//     benchmark::DoNotOptimize(selection_algorithm::standard_heap_selection(min_heap.begin(),
//     min_heap.end(), k)); benchmark::ClobberMemory();
//   }
//   state.SetComplexityN(state.range(0));
// }

static void soft_heap(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto min_heap = std::priority_queue<int, std::vector<int>, std::greater<>>(
        rand.begin(), rand.end());
    auto k = rand.size() / 2;
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        selection_algorithm::soft_heap_selection(min_heap, k));
    benchmark::ClobberMemory();
  }
}

BENCHMARK(Nth_Element)
    ->ArgsProduct({{10, 100, 1000, 10000}})
    ->Complexity(benchmark::oN);

BENCHMARK(standard_heap)
    ->ArgsProduct({{10, 100, 1000, 10000}})
    ->Complexity(benchmark::oNLogN);

BENCHMARK(standard_heap_vector)
    ->ArgsProduct({{10, 100, 1000, 10000}})
    ->Complexity(benchmark::oNLogN);

// BENCHMARK(standard_heap_iterator)
//     ->ArgsProduct({{10, 100, 1000, 10000}})
//     ->Complexity(benchmark::oNLogN);

// BENCHMARK(soft_heap)
//     ->ArgsProduct({{10, 100, 1000, 10000}})
//     ->Complexity(benchmark::oN);
