#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "src/soft_heap.hpp"
#include "src/tree.hpp"

namespace soft_heap {

auto main(int argc, char** argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace soft_heap