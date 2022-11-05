#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "soft_heap.hpp"
#include "tree.hpp"

namespace soft_heap {

SoftHeap<std::vector, int> sf(1);
TEST(SoftHeap, Epsilon) { EXPECT_EQ(sf.epsilon, 3); }
TEST(SoftHeap, R) { EXPECT_EQ(sf.r, 4); }

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
  // impl::Node<std::vector, int> node;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace soft_heap