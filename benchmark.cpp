#include "benchmark.hpp"

#include <benchmark/benchmark.h>

#include <vector>

namespace soft_heap {

static auto sizes = std::vector<int64_t>{2 << 10, 2 << 12, 2 << 14};
static void Args(benchmark::internal::Benchmark* b) {
  b->Unit(benchmark::kNanosecond)
      ->ArgsProduct({sizes})
      // ->MinTime(1)
      ->Threads(1);
}

// BENCHMARK(FlatSoftHeapConstructManual)
//     ->Unit(benchmark::kMillisecond)
//     ->UseManualTime()
//     ->ArgsProduct({sizes});
// BENCHMARK(SoftHeapConstructManual)
//     ->Unit(benchmark::kMillisecond)
//     ->UseManualTime()
//     ->ArgsProduct({sizes});
// BENCHMARK(STLHeapConstructManual)
//     ->Unit(benchmark::kMillisecond)
//     ->UseManualTime()
//     ->ArgsProduct({sizes});

// //
// BENCHMARK(FlatSoftHeapConstruct)->Unit(benchmark::kMillisecond)->ArgsProduct({sizes});
// BENCHMARK(SoftHeapConstruct)
//     ->Unit(benchmark::kMillisecond)
//     ->ArgsProduct({sizes});
// BENCHMARK(STLHeapConstruct)
//     ->Unit(benchmark::kMillisecond)
//     ->ArgsProduct({sizes});

// // BENCHMARK(FlatSoftHeapExtractManual)->ArgsProduct({sizes});
// BENCHMARK(SoftHeapExtractManual)
//     ->Unit(benchmark::kMillisecond)
//     ->MinTime(5)
//     ->UseManualTime()
//     ->ArgsProduct({sizes});
// BENCHMARK(STLHeapExtractManual)
//     ->Unit(benchmark::kMillisecond)
//     ->MinTime(5)
//     ->UseManualTime()
//     ->ArgsProduct({sizes});

// BENCHMARK(FlatSoftHeapExtractOne)->Apply(Args);
// BENCHMARK(SoftHeapExtractOne)->Apply(Args);
// BENCHMARK(STLHeapExtractOne)->Apply(Args);

BENCHMARK(FlatSoftHeapExtract)->Apply(Args);
BENCHMARK(SoftHeapExtract)->Apply(Args);
BENCHMARK(STLHeapExtract)->Apply(Args);

BENCHMARK_MAIN();

}  // namespace soft_heap
