#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "common.hpp"
#include "src/soft_heap.hpp"
#include "src/tree.hpp"

namespace soft_heap::test {

using std::vector;

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(Tree, Constructor) {
  auto tree = std::make_unique<Tree<vector, int>>(3);
  EXPECT_EQ(tree->rank, 0);
  EXPECT_EQ(tree->next, nullptr);
  EXPECT_EQ(tree->prev, nullptr);
  EXPECT_EQ(tree->suffix_min, nullptr);
  EXPECT_NODE_EQ(tree->root, Node{0, 1, vector{3}});
}

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test