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


namespace selection_algorithm {

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
    state.ResumeTiming();
    std::nth_element(rand.begin(), rand.end(), 10);
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(Nth_Element)
    ->Ranges({{10, 100, 1000, 10000}});

}

