#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

namespace soft_heap {

TEST(SoftHeap, Insert) { EXPECT_EQ(0, 0); }

auto main(int argc, char** argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace soft_heap