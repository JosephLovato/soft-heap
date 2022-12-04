#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <vector>

#include "common.hpp"
#include "node.hpp"

namespace soft_heap::test {

namespace detail {
template <int inverse_epsilon>
using NodePtr = std::unique_ptr<Node<int, std::vector<int>, inverse_epsilon>>;

template <int inverse_epsilon>
[[nodiscard]] constexpr auto MakeNodePtr(int rank, int size,
                                         auto&& list) noexcept {
  return std::make_unique<Node<int, std::vector<int>, inverse_epsilon>>(
      rank, size, std::forward<decltype(list)>(list));
}
template <int inverse_epsilon>
[[nodiscard]] constexpr auto MakeNodePtr(int element) noexcept {
  return std::make_unique<Node<int, std::vector<int>, inverse_epsilon>>(
      std::forward<decltype(element)>(element));
}

}  // namespace detail
// NOLINTBEGIN(modernize-use-trailing-return-type)

TEST(Node, Construct) { Node<int, std::vector<int>, 8> node(1); }

using ::std::list;
using ::std::vector;

TEST(Node, IsLeaf) {
  auto node = detail::MakeNodePtr<8>(4);  // move-constructor
  EXPECT_TRUE(node->IsLeaf());
  node->left = detail::MakeNodePtr<8>(1);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  EXPECT_TRUE(node->IsLeaf());
  node->right = detail::MakeNodePtr<8>(2);
  EXPECT_FALSE(node->IsLeaf());
  node->left = detail::MakeNodePtr<8>(3);
  EXPECT_FALSE(node->IsLeaf());
  node->left = nullptr;
  node->right = nullptr;
  EXPECT_TRUE(node->IsLeaf());
}

TEST(Node, Sift) {
  const auto eps = 0.1;
  const auto r = ceil(log2(1 / eps)) + 5;
  ASSERT_EQ(r, 9);
  auto node = detail::MakeNodePtr<8>(10, 2, vector{1});
  node->left = detail::MakeNodePtr<8>(9, 1, vector{2});
  node->right = detail::MakeNodePtr<8>(9, 1, vector{4});
  node->Sift();
  EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{10, 2, vector{1, 2}});
  EXPECT_NODE_EQ(node->right, Node<int, std::vector<int>, 8>{9, 1, vector{4}});
  EXPECT_EQ(node->left, nullptr);
  EXPECT_TRUE(node->right->IsLeaf());
}

TEST(Node, Combine) {
  // const auto eps = 0.1;
  // const auto r = ceil(log2(1 / eps)) + 5;
  // ASSERT_EQ(r, 9);
  auto x = detail::MakeNodePtr<8>(9, 1, vector{2});  // constructor
  auto y = detail::MakeNodePtr<8>(9, 1, vector{4});  // move-constructor
  EXPECT_TRUE(x->IsLeaf());
  EXPECT_TRUE(y->IsLeaf());
  auto node = std::make_unique<Node<int, std::vector<int>, 8>>(
      std::move(y),
      std::move(x));  // combine-constructor
  EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{10, 2, {2, 4}});
  EXPECT_TRUE(node->IsLeaf());
  EXPECT_EQ(x, nullptr);
  EXPECT_EQ(y, nullptr);
}

TEST(Node, RecursiveCombine) {
  auto left = detail::MakeNodePtr<8>(9, 1, vector{2});
  auto right = detail::MakeNodePtr<8>(9, 1, vector{4});
  auto x = detail::MakeNodePtr<8>(10, 2, vector{1, 3});
  x->left = std::move(left);
  x->right = std::move(right);
  EXPECT_NODE_EQ(x->right, Node<int, std::vector<int>, 8>{9, 1, {4}});
  EXPECT_NODE_EQ(x->left, Node<int, std::vector<int>, 8>{9, 1, {2}});
  EXPECT_NODE_EQ(x, Node<int, std::vector<int>, 8>{10, 2, {1, 3}});

  auto y = detail::MakeNodePtr<8>(9, 1, vector{5});

  auto node = std::make_unique<Node<int, std::vector<int>, 8>>(std::move(x),
                                                               std::move(y));
  EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{11, 3, {1, 3, 2, 4}});
  EXPECT_NODE_EQ(node->right, Node<int, std::vector<int>, 8>{9, 1, {5}});
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