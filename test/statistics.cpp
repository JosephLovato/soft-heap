#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <functional>
#include <queue>
#include <random>
#include <vector>

#include "common.hpp"
#include "flat_soft_heap.hpp"
#include "gtest/gtest.h"
#include "soft_heap.hpp"

namespace detail {

// credit: geeksforgeeks
// (https://www.geeksforgeeks.org/counting-inversions-using-set-in-c-stl/)
auto invariance(auto& arr) noexcept {
  auto set1 = std::multiset<int>();
  return std::accumulate(arr.begin(), arr.end(), 0, [&](auto&& sum, auto&& x) {
    set1.insert(x);
    return sum + std::distance(set1.upper_bound(x), set1.end());
  });
}

}  // namespace detail

namespace soft_heap::test {

class Environment : public ::testing::Environment {
 public:
  ~Environment() override {}

  void SetUp() override {
    fout_softheap_inv_numcorrupt << "size,epsilon,invariance,num_corrupt"
                                 << std::endl;
  }

  void TearDown() override { fout_softheap_inv_numcorrupt.close(); }

  static std::ofstream fout_softheap_inv_numcorrupt;
};

testing::Environment* const env =
    testing::AddGlobalTestEnvironment(new Environment);
std::ofstream Environment::fout_softheap_inv_numcorrupt =
    std::ofstream("soft_heap_invariance_corruptness.csv");

template <typename T>
struct SoftHeapStatistics : public ::testing::Test {};

TYPED_TEST_SUITE_P(SoftHeapStatistics);

TYPED_TEST_P(SoftHeapStatistics, InvarianceAndCorruptness) {
  for (int j = 1; j <= 10; ++j) {
    int size = j * 1000;
    auto v = detail::generate_rand(size);
    auto soft_heap = TypeParam(v.begin(), v.end());
    auto num_corrupted_keys = soft_heap.num_corrupted_keys();
    auto extracted = std::vector<int>();
    for ([[maybe_unused]] auto&& x : v) {
      extracted.push_back(soft_heap.ExtractMin());
    }
    Environment::fout_softheap_inv_numcorrupt
        << size << "," << soft_heap.epsilon << ","
        << ::detail::invariance(extracted) << "," << num_corrupted_keys
        << std::endl;
  }
}

REGISTER_TYPED_TEST_SUITE_P(SoftHeapStatistics, InvarianceAndCorruptness);
using SoftHeaps = testing::Types<
    SoftHeap<int, std::vector<int>, 2>, SoftHeap<int, std::vector<int>, 4>,
    SoftHeap<int, std::vector<int>, 8>, SoftHeap<int, std::vector<int>, 16>,
    SoftHeap<int, std::vector<int>, 32>>;
using FlatSoftHeaps = testing::Types<
    FlatSoftHeap<int, std::vector<int>, 2>, SoftHeap<int, std::vector<int>, 4>,
    SoftHeap<int, std::vector<int>, 8>, SoftHeap<int, std::vector<int>, 16>,
    SoftHeap<int, std::vector<int>, 32>>;

INSTANTIATE_TYPED_TEST_SUITE_P(SoftHeap, SoftHeapStatistics, SoftHeaps);

TEST(SoftHeap, InsertTimeOverTime) {
  std::ofstream file("soft_heap_insert_over_time.csv");
  file << "size,time_to_insert_ns" << std::endl;
  auto rand = detail::generate_rand(3000);
  auto soft_heap = SoftHeap<int, std::vector<int>, 2>(std::move(rand[0]));
  for (int i = 1; i < 3000; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    soft_heap.Insert(rand[i]);
    auto stop = std::chrono::high_resolution_clock::now();
    file << soft_heap.size() << ","
         << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
                .count();
    file << std::endl;
  }
}

TEST(SoftHeap, ExtractTimeOverTime) {
  std::ofstream file("soft_heap_extract_over_time_ns.csv");
  file << "size,time_to_extract" << std::endl;
  auto rand = detail::generate_rand(3000);
  auto soft_heap = SoftHeap<int, std::vector<int>, 2>(rand.begin(), rand.end());
  for (int i = 1; i < 3000; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    const auto& e = soft_heap.ExtractMin();
    auto stop = std::chrono::high_resolution_clock::now();
    file << soft_heap.size() << ","
         << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
                .count();
    file << std::endl;
  }
}

}  // namespace soft_heap::test