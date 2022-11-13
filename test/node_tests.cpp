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

TEST(Node, IsLeaf) {
  auto node = std::make_unique<Node<vector, int>>(4);  // move-constructor
  EXPECT_TRUE(node->IsLeaf());
  node->left = std::make_unique<Node<vector, int>>(1);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  EXPECT_TRUE(node->IsLeaf());
  node->right = std::make_unique<Node<vector, int>>(2);
  EXPECT_FALSE(node->IsLeaf());
  node->left = std::make_unique<Node<vector, int>>(3);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  node->right = nullptr;
  EXPECT_TRUE(node->IsLeaf());
}

TEST(Node, Sift) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto node = std::make_unique<Node<vector, int>>(10, 2, vector{1});
  node->left = std::make_unique<Node<vector, int>>(9, 1, vector{2});
  node->right = std::make_unique<Node<vector, int>>(9, 1, vector{4});
  EXPECT_NODE_EQ(node->right, Node{9, 1, vector{4}});
  EXPECT_TRUE(node->right->IsLeaf());
  EXPECT_NODE_EQ(node->left, Node(9, 1, vector{2}));
  EXPECT_TRUE(node->left->IsLeaf());
  EXPECT_NODE_EQ(node, Node{10, 2, vector{1}});
  node->Sift();
  EXPECT_NODE_EQ(node, Node{10, 2, vector{1, 2}});
  EXPECT_EQ(node->left, nullptr);
  EXPECT_NODE_EQ(node->right, Node{9, 1, vector{4}});
  EXPECT_TRUE(node->right->IsLeaf());
}

TEST(Node, Combine) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  const int n = 2;
  auto x = std::make_unique<Node<vector, int>>(n);  // constructor
  auto y = std::make_unique<Node<vector, int>>(4);  // move-constructor
  x->rank = 9;
  y->rank = 9;
  EXPECT_NODE_EQ(x, Node{9, 1, vector{2}});
  EXPECT_TRUE(x->IsLeaf());
  EXPECT_NODE_EQ(y, Node{9, 1, vector{4}});
  EXPECT_TRUE(y->IsLeaf());
  auto node = std::make_unique<Node<vector, int>>(x, y,
                                                  r);  // combine-constructor
  EXPECT_NODE_EQ(node, Node{10, 2, vector{2, 4}});
  EXPECT_TRUE(node->IsLeaf());
  EXPECT_EQ(x, nullptr);
  EXPECT_EQ(y, nullptr);
}

TEST(Node, RecursiveCombine) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto left = std::make_unique<Node<vector, int>>(9, 1, vector{2});
  auto right = std::make_unique<Node<vector, int>>(9, 1, vector{4});
  auto x = std::make_unique<Node<vector, int>>(
      10, 2, vector{1, 3}, std::move(left),
      std::move(right));  // move-constructor
  EXPECT_NODE_EQ(x->right, Node{9, 1, vector{4}});
  EXPECT_NODE_EQ(x->left, Node{9, 1, vector{2}});
  EXPECT_NODE_EQ(x, Node{10, 2, vector{1, 3}});

  auto y = std::make_unique<Node<vector, int>>(5);
  y->rank = 9;

  auto node = std::make_unique<Node<vector, int>>(x, y, r);
  EXPECT_NODE_EQ(node, Node{11, 3, vector{1, 3, 2, 4}});
  EXPECT_NODE_EQ(node->right, Node{9, 1, vector{5}});
  EXPECT_EQ(node->left, nullptr);
  EXPECT_EQ(y, nullptr);
}

// NOLINTEND(modernize-use-trailing-return-type)
}  // namespace soft_heap::test