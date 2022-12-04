#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <memory>
#include <vector>

#include "common.hpp"
#include "tree.hpp"

namespace soft_heap::test {

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(Tree, Construct) { auto tree = Tree<int, std::vector<int>, 8>(0); }

TEST(Tree, Constructor) {
  auto tree = std::make_unique<Tree<int, std::vector<int>, 8>>(3);
  EXPECT_EQ(tree->rank(), 0);
  EXPECT_NODE_EQ(tree->root, Node<int, std::vector<int>, 8>{0, 1, {3}});
}

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test
