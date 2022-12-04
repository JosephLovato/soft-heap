#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <functional>
#include <queue>
#include <random>
#include <vector>

#include "flat_soft_heap.hpp"

namespace soft_heap::test {

namespace detail {

// [[nodiscard]] auto generate_rand(int n) noexcept {
//   auto v = std::vector<int>(n);
//   std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
//   std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
//   return v;
// }

}  // namespace detail
   //
// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(FlatSoftHeap, Construct) {
  auto soft_heap = FlatSoftHeap<int, 8>(0);
  // EXPECT_EQ(node->rank, expect.rank);
}

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test

// #include <gmock/gmock.h>
// #include <gtest/gtest.h>

// #include <algorithm>
// #include <fstream>
// #include <iostream>
// #include <memory>
// #include <queue>
// #include <random>
// #include <set>
// #include <vector>

// #include "common.hpp"
// #include "soft_heap.hpp"
// #include "tree.hpp"

// namespace soft_heap::test {

// namespace detail {

// // credit: geeksforgeeks
// // (https://www.geeksforgeeks.org/counting-inversions-using-set-in-c-stl/)
// auto invarience(auto& arr) noexcept {
//   auto set1 = std::multiset<int>();
//   return std::accumulate(arr.begin(), arr.end(), 0, [&](auto&& sum, auto&& x)
//   {
//     set1.insert(x);
//     return sum + std::distance(set1.upper_bound(x), set1.end());
//   });
// }

// }  // namespace detail

// // NOLINTBEGIN(modernize-use-trailing-return-type)

// TEST(SoftHeap, SimpleSortedness) {
//   auto fout = std::ofstream("soft_heap_invarience.txt");
//   fout << "size, epsilon, invarience" << std::endl;
//   for (int i = 0; i <= 9; ++i) {
//     for (int j = 1; j <= 10; ++j) {
//       int size = j * 1000;
//       double epsilon = 0.01 + (i * 0.05);
//       auto v = detail::generate_rand(size);
//       auto soft_heap = SoftHeap<std::vector, int>(v.begin(), v.end(),
//       epsilon); auto extracted = std::vector<int>(); for ([[maybe_unused]]
//       auto&& x : v) {
//         extracted.push_back(soft_heap.ExtractMin());
//       }
//       fout << size << "," << epsilon << "," << detail::invarience(extracted)
//            << std::endl;
//     }
//     fout << std::endl;
//   }
// }

// TEST(SoftHeap, SimpleCorruptedness) {
//   auto fout = std::ofstream("soft_heap_corruptedness.txt");
//   fout << "size,epsilon,num_corrupted" << std::endl;
//   for (int i = 0; i <= 9; ++i) {
//     for (int j = 1; j <= 10; ++j) {
//       int size = j * 1000;
//       double epsilon = 0.01 + (i * 0.05);
//       auto v = detail::generate_rand(size);
//       auto soft_heap = SoftHeap<std::vector, int>(v.begin(), v.end(),
//       epsilon); fout << size << "," << epsilon << "," <<
//       soft_heap.num_corrupted_keys()
//            << std::endl;
//     }
//     fout << std::endl;
//   }
// }
// // NOLINTEND(modernize-use-trailing-return-type)

// }  // namespace soft_heap::test