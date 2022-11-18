#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "common.hpp"
#include "src/soft_heap.hpp"
#include "src/tree.hpp"
namespace soft_heap::test {

using std::vector;

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(SoftHeap, Print) {
  const auto rand = [] {
    auto v = std::vector<int>(10);
    std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
    std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
    return v;
  }();  // rand

  auto soft_heap = SoftHeap<std::vector, int>(rand.begin(), rand.end(), 1);

  auto fout = std::ofstream("soft_heap_print.txt", std::ios::out);
  fout << soft_heap;
}

TEST(SoftHeap, ExtractMinPrint) {
  while (true) {
    const auto rand = [] {
      auto v = std::vector<int>(10);
      std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
      std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
      return v;
    }();  // rand
    auto soft_heap = SoftHeap<std::vector, int>(rand.begin(), rand.end(), 0.99);

    // fout << soft_heap;
    // auto cout = std::ofstream("extract_min.txt", std::ios::out);
    auto fout = std::ofstream("soft_heap_extract.txt", std::ios::out);
    for (int i = 0; i < rand.size(); i++) {
      const auto elem = soft_heap.ExtractMin();
      fout << elem << ", ";
      // fout << "After extracting: " << elem << "\n" << soft_heap;
    }
    fout << std::endl;
    // std::cout << "\n";
  }
}

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test