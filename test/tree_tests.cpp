#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <memory>
#include <vector>

#include "common.hpp"
#include "tree.hpp"

namespace soft_heap::test {

using ::std::list;
using ::std::vector;

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(Tree, Construct) { Tree<int> tree{0}; }

TEST(Tree, Constructor) {
  auto tree = std::make_unique<Tree<int>>(3);
  EXPECT_EQ(tree->rank(), 0);
  // EXPECT_EQ(tree->min_ckey, nullptr);
  // EXPECT_TRE(tree->suffix_min, nullptr);
  EXPECT_NODE_EQ(tree->root, Node<int>{0, 1, vector{3}});
}

// TEST(Tree, UpdateSuffixMin) {
//   Tree<int> tree{0};
//   tree.next = std::make_shared<Tree<int>>(1);
//   tree.UpdateSuffixMin();
// }

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace soft_heap::test

// #include <gmock/gmock.h>
// #include <gtest/gtest.h>

// #include <algorithm>
// #include <cstddef>
// #include <fstream>
// #include <iostream>
// #include <memory>
// #include <vector>

// #include "common.hpp"
// #include "soft_heap.hpp"
// #include "tree.hpp"

// namespace soft_heap::test {

// using std::vector;

// // NOLINTBEGIN(modernize-use-trailing-return-type)

// TEST(Tree, Constructor) {
//   auto tree = std::make_unique<Tree<vector, int>>(3);
//   EXPECT_EQ(tree->rank(), 0);
//   EXPECT_EQ(tree->next, nullptr);
//   EXPECT_EQ(tree->prev, nullptr);
//   // EXPECT_TRE(tree->suffix_min, nullptr);
//   EXPECT_NODE_EQ(tree->root, Node{0, 1, vector{3}});
// }

// // TEST(Tree, Print) {
// //   auto node = std::make_unique<Node<std::vector, int>>(1);
// //   node->elements = {1, 2, 3, 4, 5};
// //   node->left = std::make_unique<Node<std::vector, int>>(7);
// //   node->right = std::make_unique<Node<std::vector, int>>(8);

// //   auto tree = Tree<std::vector, int>(1);
// //   tree.root = std::move(node);

// //   auto fout = std::ofstream("tree_print.txt", std::ios::out);
// //   fout << tree;
// //   EXPECT_EQ(1, 1);
// // }

// // NOLINTEND(modernize-use-trailing-return-type)

// }  // namespace soft_heap::test