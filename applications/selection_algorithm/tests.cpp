#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <random>
#include <vector>

#include "selection_algorithm.hpp"

namespace selection_algorithm::test {

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

TEST(Selection, Standard_Heap) {
  const size_t k = 200;
  auto input_heap = bench::generate_rand(1000);
  std::make_heap(input_heap.begin(), input_heap.end(), std::greater<>{});
  std::priority_queue<int, std::vector<int>, std::greater<>> min_heap(
      input_heap.begin(), input_heap.end());

  auto k_elements = selection_algorithm::standard_heap_selection(input_heap, k);

  ASSERT_EQ(k_elements.size(), k);
  for (size_t i = 0; i < k; i++) {
    if (min_heap.top() != k_elements.at(i)) {
      FAIL();
    }
    min_heap.pop();
  }
}

// TEST(Selection, Standard_Heap_Iterator) {
//   const size_t k = 200;
//   auto input_heap = bench::generate_rand(1000);
//   std::make_heap(input_heap.begin(), input_heap.end(), std::greater<>{});
//   std::priority_queue<int, std::vector<int>, std::greater<>>
//   min_heap(input_heap.begin(), input_heap.end());

//   auto k_elements =
//   selection_algorithm::standard_heap_selection(input_heap.begin(),
//   input_heap.end(), k);

//   ASSERT_EQ(k_elements.size(), k);
//   for (size_t i = 0; i < k; i++) {
//     if (min_heap.top() != k_elements.at(i)) {
//       FAIL();
//     }
//     min_heap.pop();
//   }
// }

TEST(Selection, Soft_Heap) {
  const size_t k = 200;
  auto input_heap = bench::generate_rand(1000);
  std::make_heap(input_heap.begin(), input_heap.end(), std::greater<>{});
  std::priority_queue<int, std::vector<int>, std::greater<>> min_heap(
      input_heap.begin(), input_heap.end());

  auto k_elements = selection_algorithm::soft_heap_selection(input_heap, k);

  std::sort(k_elements.begin(), k_elements.end());

  ASSERT_EQ(k_elements.size(), k);

  for (size_t i = 0; i < k; i++) {
    if (min_heap.top() != k_elements.at(i)) {
      FAIL();
    }
    min_heap.pop();
  }
}
}  // namespace selection_algorithm::test