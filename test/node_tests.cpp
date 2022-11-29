#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <vector>

#include "common.hpp"
#include "node.hpp"

namespace soft_heap::test {

// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(Node, Construct) { Node<int> node(1); }

using ::std::list;

TEST(Node, IsLeaf) {
  auto node = std::make_unique<Node<int>>(4);  // move-constructor
  EXPECT_TRUE(node->IsLeaf());
  node->left = std::make_unique<Node<int>>(1);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  EXPECT_TRUE(node->IsLeaf());
  node->right = std::make_unique<Node<int>>(2);
  EXPECT_FALSE(node->IsLeaf());
  node->left = std::make_unique<Node<int>>(3);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  node->right = nullptr;
  EXPECT_TRUE(node->IsLeaf());
}

TEST(Node, Sift) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto node = std::make_unique<Node<int>>(10, 2, list{1});
  node->left = std::make_unique<Node<int>>(9, 1, list{2});
  node->right = std::make_unique<Node<int>>(9, 1, list{4});
  node->Sift();
  EXPECT_NODE_EQ(node, Node<int>{10, 2, list{1, 2}});
  EXPECT_NODE_EQ(node->right, Node<int>{9, 1, list{4}});
  EXPECT_EQ(node->left, nullptr);
  EXPECT_TRUE(node->right->IsLeaf());
}

TEST(Node, Combine) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto x = std::make_unique<Node<int>>(9, 1, list{2});  // constructor
  auto y = std::make_unique<Node<int>>(9, 1, list{4});  // move-constructor
  EXPECT_TRUE(x->IsLeaf());
  EXPECT_TRUE(y->IsLeaf());
  auto node = std::make_unique<Node<int>>(std::move(y), std::move(x),
                                          r);  // combine-constructor
  EXPECT_NODE_EQ(node, Node<int>{10, 2, list{2, 4}});
  EXPECT_TRUE(node->IsLeaf());
  EXPECT_EQ(x, nullptr);
  EXPECT_EQ(y, nullptr);
}

TEST(Node, RecursiveCombine) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto left = std::make_unique<Node<int>>(9, 1, list{2});
  auto right = std::make_unique<Node<int>>(9, 1, list{4});
  auto x = std::make_unique<Node<int>>(10, 2, list{1, 3});
  x->left = std::move(left);
  x->right = std::move(right);
  EXPECT_NODE_EQ(x->right, Node<int>{9, 1, list{4}});
  EXPECT_NODE_EQ(x->left, Node<int>{9, 1, list{2}});
  EXPECT_NODE_EQ(x, Node<int>{10, 2, list{1, 3}});

  auto y = std::make_unique<Node<int>>(9, 1, list{5});

  auto node = std::make_unique<Node<int>>(std::move(x), std::move(y), r);
  EXPECT_NODE_EQ(node, Node<int>{11, 3, list{1, 3, 2, 4}});
  EXPECT_NODE_EQ(node->right, Node<int>{9, 1, list{5}});
  EXPECT_EQ(node->left, nullptr);
  EXPECT_EQ(y, nullptr);
}

// TEST(Node, Print) {
//   auto node2 = Node<std::vector, int>(1);
//   node2.elements = {1, 2, 3, 4, 5};

//   auto fout = std::ofstream("node_print.txt", std::ios::out);
//   fout << node2;
//   EXPECT_EQ(1, 1);
// }

// NOLINTEND(modernize-use-trailing-return-type)
}  // namespace soft_heap::test