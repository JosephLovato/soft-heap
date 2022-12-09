#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <functional>
#include <queue>
#include <random>
#include <vector>

#include "soft_heap.hpp"

namespace soft_heap::test {

namespace detail {

[[nodiscard]] auto generate_rand(int n) noexcept {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
  std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
  return v;
}

}  // namespace detail

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(SoftHeap, Construct) { SoftHeap<int> soft_heap{0}; }

TEST(SoftHeap, STLConstruct) {
//  auto rand = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto rand = detail::generate_rand(1000000);
  std::sort(rand.begin(), rand.end());
  auto soft_heap = SoftHeap<int, std::vector<int>, 2>{rand.begin(), rand.end()};
  int corrupted = soft_heap.num_corrupted_keys();
  std::cout << "Num corrupted: " << corrupted;
  auto fout = std::ofstream("soft_heap.txt");
  fout << soft_heap;
}

TEST(SoftHeap, Extract) {
  // auto rand = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto rand = detail::generate_rand(50);
  auto soft_heap =
      SoftHeap<int, std::vector<int>, 10>{std::make_move_iterator(rand.begin()),
                                          std::make_move_iterator(rand.end())};
  auto fout = std::ofstream("soft_heap.txt");
  for ([[maybe_unused]] auto&& x : rand) {
    fout << soft_heap.ExtractMin() << ',';
  }
  fout << std::endl;
}

TEST(SoftHeap, ExtractMin) {
  auto rand = detail::generate_rand(3000);
  auto soft_heap =
      // SoftHeap<int, std::vector<int>, 10>(rand.begin(), rand.end());
      SoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
  auto stl_heap =
      std::priority_queue(rand.begin(), rand.end(), std::greater<>());
  auto fout = std::ofstream("soft_heap_extract.txt");

  while (not stl_heap.empty()) {
    const auto sh_elem = soft_heap.ExtractMin();
    const auto stl_elem = stl_heap.top();
    stl_heap.pop();
    EXPECT_EQ(sh_elem, stl_elem);
    fout << sh_elem << ',';
  }
}

TEST(SoftHeap, ExtractMinVerifyAllElements) {
  const int inverse_eps = 4;
  auto rand = detail::generate_rand(3000);
  auto soft_heap =
      SoftHeap<int, std::vector<int>, inverse_eps>(rand.begin(), rand.end());

  auto fout = std::ofstream("soft_heap_extract_verify_all_elements.txt");

  std::set<int> extracted_elems;
  for (int i = 0; i < rand.size(); i++) {
    const auto sh_elem = soft_heap.ExtractMin();
    if (!extracted_elems.insert(sh_elem).second) {
      fout << sh_elem << ',';
      FAIL();
    }
  }
}

TEST(SoftHeap, TreeListSize) {
  std::ofstream file("tree_list_size.txt");
  file << "soft_heap_size,tree_list_size" << std::endl;
  for (int i = 1; i <= 10; i++) {
    auto size = i * 1000;
    auto rand = detail::generate_rand(size);
    auto soft_heap =
        SoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
    file << size << "," << soft_heap.trees.size() << std::endl;
  }
}

TEST(SoftHeap, TreeListSizeOverTime) {
  std::ofstream file("tree_list_over_time.txt");
  file << "insert_number,tree_list_size,ranks" << std::endl;
  auto rand = detail::generate_rand(10000);
  auto soft_heap = SoftHeap<int, std::vector<int>, 2>(std::move(rand[0]));
  for (int i = 1; i < 10000; i++) {
    soft_heap.Insert(rand[i]);
    if (i % 1 == 0) {
      file << i << "," << soft_heap.trees.size() << "::";
      for (const auto& t : soft_heap.trees) {
        file << t.rank() << ",";
      }
      file << std::endl;
    }
  }
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