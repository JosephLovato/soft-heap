#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "soft_heap.hpp"
#include "tree.hpp"

namespace soft_heap {

SoftHeap<std::vector, int> sf(1);
TEST(SoftHeap, Epsilon) { EXPECT_NE(sf.epsilon, 3); }
TEST(SoftHeap, R) { EXPECT_NE(sf.r, 4); }

Tree<std::vector, int> tree(2);
TEST(Tree, rank) { EXPECT_EQ(tree.rank, 0); }
TEST(Tree, next) { EXPECT_EQ(tree.next, nullptr); }
TEST(Tree, prev) { EXPECT_EQ(tree.prev, nullptr); }
TEST(Tree, suff) { EXPECT_EQ(tree.suffix_min, nullptr); }
TEST(Tree, size) { EXPECT_EQ(tree.root->size, 1); }
TEST(Tree, ckey) { EXPECT_EQ(tree.root->ckey, 2); }

SoftHeap<std::vector, int> sh(2);
TEST(SoftHeap, rank) { EXPECT_EQ(sh.rank, 0); }
TEST(SoftHeap, next) { EXPECT_EQ(sh.first_tree->next, nullptr); }
TEST(SoftHeap, prev) { EXPECT_EQ(sh.first_tree->prev, nullptr); }
TEST(SoftHeap, suff) { EXPECT_EQ(sh.first_tree->suffix_min, nullptr); }
TEST(SoftHeap, size) { EXPECT_EQ(sh.first_tree->root->size, 1); }
TEST(SoftHeap, ckey) { EXPECT_EQ(sh.first_tree->root->ckey, 2); }

auto main(int argc, char** argv) -> int {
  auto tree = std::make_shared<Tree<std::vector, int>>(2);
  SoftHeap<std::vector, int>::UpdateSuffixMin(tree);

  auto node1 = std::make_unique<Node<std::vector, int>>(2);
  auto node2 = std::make_unique<Node<std::vector, int>>(3);
  auto node3 = std::make_unique<Node<std::vector, int>>(std::move(node1),
                                                        std::move(node2), 1);
  node3->IsLeaf();
  node3->Sift();

  auto soft_heap1 = std::make_unique<SoftHeap<std::vector, int>>(2);
  std::vector v = {1, 2, 3, 4};
  auto soft_heap2 =
      std::make_unique<SoftHeap<std::vector, int>>(v.begin(), v.end());
  [[maybe_unused]] auto elem = soft_heap2->ExtractMin();

  // impl::Node<std::vector, int> node;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace soft_heap