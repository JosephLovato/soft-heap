#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "common.hpp"
#include "src/soft_heap.hpp"
#include "src/tree.hpp"

namespace soft_heap::test {

using std::vector;

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(SoftHeap, IterConstructor) {
  const auto rand_vect = std::vector{
      45, 31, 10, 46, 33, 22, 35, 42, 9,  38, 32, 23, 19, 13, 21, 4,  50,
      14, 11, 37, 41, 3,  1,  36, 26, 20, 30, 43, 49, 48, 16, 39, 24, 27,
      47, 6,  17, 25, 2,  8,  15, 5,  44, 7,  18, 34, 29, 12, 28, 40};
}

TEST(SoftHeap, Print) {
  const auto rand_vect = std::vector{45, 31, 10, 46, 33, 22, 35, 42, 9};
  auto soft_heap =
      SoftHeap<std::vector, int>(rand_vect.begin(), rand_vect.end(), 1);

  auto fout = std::ofstream("soft_heap_print.txt", std::ios::out);
  fout << soft_heap;
}

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test