#include <benchmark/benchmark.h>

#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "flat_soft_heap.hpp"
#include "node.hpp"
#include "soft_heap.hpp"
#include "tree.hpp"

namespace soft_heap {

namespace bench {

[[nodiscard]] inline auto generate_rand(int n) noexcept {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
  std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
  return v;
}

[[nodiscard]] inline auto rand_int(int start, int stop) noexcept {
  auto generator = std::mt19937(std::random_device()());
  auto dist = std::uniform_int_distribution<int>(start, stop);
  return dist(generator);
}

}  // namespace bench

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void FlatSoftHeapConstructManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    const auto start = std::chrono::high_resolution_clock::now();
    benchmark::DoNotOptimize(
        FlatSoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end()));
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void SoftHeapConstructManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    const auto start = std::chrono::high_resolution_clock::now();
    benchmark::DoNotOptimize(
        SoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end()));
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

[[maybe_unused]] static void STLHeapConstructManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    const auto start = std::chrono::high_resolution_clock::now();
    benchmark::DoNotOptimize(
        std::priority_queue<int, std::vector<int>, std::greater<>>(
            rand.begin(), rand.end(), std::greater<>()));
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void FlatSoftHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        FlatSoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end()));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void SoftHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        SoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end()));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

[[maybe_unused]] static void STLHeapConstruct(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        std::priority_queue<int, std::vector<int>, std::greater<>>(
            rand.begin(), rand.end(), std::greater<>()));
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(state.range(0));
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void SoftHeapExtractManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    const auto start = std::chrono::high_resolution_clock::now();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(soft_heap.ExtractMin());
      benchmark::ClobberMemory();
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void FlatSoftHeapExtractManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        FlatSoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    const auto start = std::chrono::high_resolution_clock::now();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(soft_heap.ExtractMin());
      benchmark::ClobberMemory();
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
  }
}

[[maybe_unused]] static void STLHeapExtractManual(benchmark::State& state) {
  for (auto _ : state) {
    auto rand = bench::generate_rand(state.range(0));
    std::priority_queue min_queue(rand.begin(), rand.end(), std::greater<>());
    const auto start = std::chrono::high_resolution_clock::now();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::ClobberMemory();
      benchmark::DoNotOptimize(min_queue.top());
      benchmark::ClobberMemory();
      min_queue.pop();
      benchmark::ClobberMemory();
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void SoftHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    state.ResumeTiming();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(soft_heap.ExtractMin());
      benchmark::ClobberMemory();
    }
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void FlatSoftHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        FlatSoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    state.ResumeTiming();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(soft_heap.ExtractMin());
      benchmark::ClobberMemory();
    }
  }
}

[[maybe_unused]] static void STLHeapExtract(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    std::priority_queue min_queue(rand.begin(), rand.end(), std::greater<>());
    state.ResumeTiming();
    for ([[maybe_unused]] auto&& x : rand) {
      benchmark::DoNotOptimize(min_queue.top());
      benchmark::ClobberMemory();
      min_queue.pop();
      benchmark::ClobberMemory();
    }
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void SoftHeapExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        SoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    state.ResumeTiming();
    benchmark::DoNotOptimize(soft_heap.ExtractMin());
    benchmark::ClobberMemory();
  }
}

template <class List = std::vector<int>, int inverse_epsilon = 8>
static void FlatSoftHeapExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    auto soft_heap =
        FlatSoftHeap<int, List, inverse_epsilon>(rand.begin(), rand.end());
    state.ResumeTiming();
    benchmark::DoNotOptimize(soft_heap.ExtractMin());
    benchmark::ClobberMemory();
  }
}

[[maybe_unused]] static void STLHeapExtractOne(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto rand = bench::generate_rand(state.range(0));
    std::priority_queue min_queue(rand.begin(), rand.end(), std::greater<>());
    state.ResumeTiming();
    benchmark::DoNotOptimize(min_queue.top());
    benchmark::ClobberMemory();
    min_queue.pop();
    benchmark::ClobberMemory();
  }
}

// template <class List = std::vector<int>, int inverse_epsilon = 8>
// static void SoftHeapInsert(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand = bench::generate_rand(state.range(0));
//     auto rand_int = bench::rand_int(1, 100000);
//     auto soft_heap = SoftHeap<int, List, inverse_epsilon>(
//         rand.begin(), std::next(rand.begin(), state.range(0)));
//     state.ResumeTiming();
//     // for (int i = 0; i < 1000; ++i) {
//     soft_heap.Insert(rand_int);
//     // }
//   }
//   state.SetComplexityN(state.range(0));
// }

// static void STLHeapInsert(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand = bench::generate_rand(state.range(0));
//     const auto rand_int = bench::rand_int(1, 100000);
//     auto min_queue = std::priority_queue<int>(rand.begin(),
//     rand.end(),std::greater<int>()); state.ResumeTiming();
//     benchmark::ClobberMemory();
//     // for (int i = 0; i < 1000; ++i) {
//     min_queue.push(rand_int);
//     // }
//     benchmark::ClobberMemory();
//   }
//   state.SetComplexityN(state.range(0));
// }

// static void SoftHeapExtractOne8(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand = bench::generate_rand(state.range(0));
//     auto soft_heap = SoftHeap<int>(rand.begin(), rand.end());
//     state.ResumeTiming();
//     benchmark::DoNotOptimize(soft_heap.ExtractMin());
//     benchmark::ClobberMemory();
//   }
//   state.SetComplexityN(state.range(0));
// }
// static void STLHeapExtractOne(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand = bench::generate_rand(state.range(0));
//     auto min_queue = std::priority_queue<int>(rand.begin(),
//     rand.end(),std::greater<int>()); state.ResumeTiming();
//     benchmark::DoNotOptimize(min_queue.top());
//     benchmark::ClobberMemory();
//     min_queue.pop();
//     benchmark::ClobberMemory();
//   }
//   state.SetComplexityN(state.range(0));
// }
//
// static void VectorSortExtractOne(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     auto rand_in = bench::generate_rand(state.range(0));
//     state.ResumeTiming();
//     auto rand = std::vector(rand_in.begin(), rand_in.end());
//     std::sort(rand.begin(), rand.end(), std::less<>());
//     benchmark::DoNotOptimize(rand.back());
//     benchmark::ClobberMemory();
//     rand.erase(rand.begin());
//     benchmark::ClobberMemory();
//   }
//   // state.SetComplexityN(state.range(0));
// }

}  // namespace soft_heap
