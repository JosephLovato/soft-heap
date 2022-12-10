#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <list>
#include <vector>

#include "common.hpp"
#include "flat_node.hpp"
#include "flat_soft_heap.hpp"
#include "flat_tree.hpp"
#include "soft_heap.hpp"
// NOLINTBEGIN(modernize-use-trailing-return-type)

namespace soft_heap::test {

using ::std::vector;
using N = FlatNode<int, vector<int>, 8>;
using T = FlatTree<int, vector<int>, 8>;

// TEST(FlatNode, Construct) { Node<int, std::vector<int>, 8> node(1); }

// TEST(FlatTree, Construct) { auto tree = Tree<int, std::vector<int>, 8>(0); }

// TEST(FlatTree, Constructor) {
//   auto tree = T(3);
//   EXPECT_EQ(tree.rank(), 0);
//   EXPECT_NODE_EQ(tree.node_heap[0], N{0, 1, {3}});
// }

// TEST(FlatSoftHeap, Construct) { FlatSoftHeap<int> Heapsoft_heap{0}; }

// TEST(FlatSoftHeap, STLConstruct) {
//   auto rand = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//   auto soft_heap =
//       FlatSoftHeap<int, std::vector<int>, 2>{rand.begin(), rand.end()};
//   auto fout = std::ofstream("flat_soft_heap.txt");
//   fout << soft_heap;
// }

// TEST(FlatSoftHeapCompare, CompareToSoftHeap) {
//   auto rand = detail::generate_rand(3000);
//   auto soft_heap =
//       // SoftHeap<int, std::vector<int>, 10>(rand.begin(), rand.end());
//       SoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
//   auto flat_soft_heap =
//       // SoftHeap<int, std::vector<int>, 10>(rand.begin(), rand.end());
//       FlatSoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
//   // auto stl_heap =
//   //     std::priority_queue(rand.begin(), rand.end(), std::greater<>());
//   // auto fout = std::ofstream("soft_heap_extract_min.txt");

//   EXPECT_EQ(soft_heap.rank(), flat_soft_heap.rank());
//   ASSERT_EQ(soft_heap.trees.size(), flat_soft_heap.trees.size());
//   for (int i = 0; i < std::ssize(soft_heap.trees); ++i) {
//     EXPECT_EQ(std::next(soft_heap.trees.begin(), i)->rank(),
//               std::next(flat_soft_heap.trees.begin(), i)->rank());
//   }
// }

TEST(FlatSoftHeap, Extract) {
  // auto rand = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto rand = detail::generate_rand(2000);
  auto soft_heap = FlatSoftHeap<int, std::vector<int>, 1>{
      std::make_move_iterator(rand.begin()),
      std::make_move_iterator(rand.end())};
  EXPECT_EQ(2000, soft_heap.size());
  auto fout = std::ofstream("./flat_soft_heap_extract_min2.txt");
  for ([[maybe_unused]] auto&& x : rand) {
   /* std::cerr << */ soft_heap.ExtractMin(); //<< std::endl;
  }
  fout << std::endl;
  EXPECT_EQ(0, soft_heap.size());
}

// TEST(FlatSoftHeap, ExtractUniqueness) {
//   // auto rand = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//   auto rand = detail::generate_rand(10000);
//   auto soft_heap = FlatSoftHeap<int, std::vector<int>, 8>{
//       std::make_move_iterator(rand.begin()),
//       std::make_move_iterator(rand.end())};
//   auto fout = std::ofstream("flat_soft_heap_extract_min.txt");
//   for ([[maybe_unused]] auto&& x : rand) {
//     fout << soft_heap.ExtractMin() << std::endl;
//   }
//   fout << std::endl;
// }

TEST(FlatSoftHeapCompare, ExtractCompare) {
  auto rand = detail::generate_rand(2000);
  auto soft_heap =
      // SoftHeap<int, std::vector<int>, 10>(rand.begin(), rand.end());
      SoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
  auto flat_soft_heap =
      // SoftHeap<int, std::vector<int>, 10>(rand.begin(), rand.end());
      FlatSoftHeap<int, std::vector<int>, 1000>(rand.begin(), rand.end());
  // auto stl_heap =
  //     std::priority_queue(rand.begin(), rand.end(), std::greater<>());
  // auto fout = std::ofstream("soft_heap_extract_min.txt");

  EXPECT_EQ(soft_heap.rank(), flat_soft_heap.rank());
  ASSERT_EQ(soft_heap.trees.size(), flat_soft_heap.trees.size());
  for (int i = 0; i < std::ssize(soft_heap.trees); ++i) {
    EXPECT_EQ(soft_heap.ExtractMin(), flat_soft_heap.ExtractMin());
  }
}

TEST(FlatSoftHeap, ExtractMinVerifyAllElements) {
  const int inverse_eps = 4;
  auto rand = detail::generate_rand(3000);
  auto soft_heap = FlatSoftHeap<int, std::vector<int>, inverse_eps>(
      rand.begin(), rand.end());
  EXPECT_EQ(3000, soft_heap.size());
  // auto fout = std::ofstream("soft_heap_extract_verify_all_elements.txt");

  std::set<int> extracted_elems;
  for (int i = 0; i < std::ssize(rand); i++) {
    const auto sh_elem = soft_heap.ExtractMin();
    if (!extracted_elems.insert(sh_elem).second) {
      // fout << sh_elem << ',';
      FAIL();
    }
  }
  EXPECT_EQ(0, soft_heap.size());
}

// TEST(FlatNode, IsLeaf) {
//   constexpr int inv_eps = 8;
//   auto node = N(4);  // move-constructor
//   EXPECT_TRUE(node->IsLeaf());
//   node->left = detail::MakeNodePtr<8>(1);
//   EXPECT_FALSE(node->IsLeaf());
//   node->left = nullptr;
//   EXPECT_TRUE(node->IsLeaf());
//   node->right = detail::MakeNodePtr<8>(2);
//   EXPECT_FALSE(node->IsLeaf());
//   node->left = detail::MakeNodePtr<8>(3);
//   EXPECT_FALSE(node->IsLeaf());
//   node->left = nullptr;
//   node->right = nullptr;
//   EXPECT_TRUE(node->IsLeaf());
// }

// TEST(Node, Sift) {
//   const auto eps = 0.1;
//   const auto r = ceil(log2(1 / eps)) + 5;
//   ASSERT_EQ(r, 9);
//   auto node = detail::MakeNodePtr<8>(10, 2, vector{1});
//   node->left = detail::MakeNodePtr<8>(9, 1, vector{2});
//   node->right = detail::MakeNodePtr<8>(9, 1, vector{4});
//   node->Sift();
//   EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{10, 2, vector{1, 2}});
//   EXPECT_NODE_EQ(node->right, Node<int, std::vector<int>, 8>{9, 1,
//   vector{4}}); EXPECT_EQ(node->left, nullptr);
//   EXPECT_TRUE(node->right->IsLeaf());
// }

// TEST(Node, Combine) {
//   // const auto eps = 0.1;
//   // const auto r = ceil(log2(1 / eps)) + 5;
//   // ASSERT_EQ(r, 9);
//   auto x = detail::MakeNodePtr<8>(9, 1, vector{2});  // constructor
//   auto y = detail::MakeNodePtr<8>(9, 1, vector{4});  // move-constructor
//   EXPECT_TRUE(x->IsLeaf());
//   EXPECT_TRUE(y->IsLeaf());
//   auto node = std::make_unique<Node<int, std::vector<int>, 8>>(
//       std::move(y),
//       std::move(x));  // combine-constructor
//   EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{10, 2, {2, 4}});
//   EXPECT_TRUE(node->IsLeaf());
//   EXPECT_EQ(x, nullptr);
//   EXPECT_EQ(y, nullptr);
// }

// TEST(Node, RecursiveCombine) {
//   auto left = detail::MakeNodePtr<8>(9, 1, vector{2});
//   auto right = detail::MakeNodePtr<8>(9, 1, vector{4});
//   auto x = detail::MakeNodePtr<8>(10, 2, vector{1, 3});
//   x->left = std::move(left);
//   x->right = std::move(right);
//   EXPECT_NODE_EQ(x->right, Node<int, std::vector<int>, 8>{9, 1, {4}});
//   EXPECT_NODE_EQ(x->left, Node<int, std::vector<int>, 8>{9, 1, {2}});
//   EXPECT_NODE_EQ(x, Node<int, std::vector<int>, 8>{10, 2, {1, 3}});

//   auto y = detail::MakeNodePtr<8>(9, 1, vector{5});

//   auto node = std::make_unique<Node<int, std::vector<int>, 8>>(std::move(x),
//                                                                std::move(y));
//   EXPECT_NODE_EQ(node, Node<int, std::vector<int>, 8>{11, 3, {1, 3, 2, 4}});
//   EXPECT_NODE_EQ(node->right, Node<int, std::vector<int>, 8>{9, 1, {5}});
//   EXPECT_EQ(node->left, nullptr);
//   EXPECT_EQ(y, nullptr);
// }

// TEST(Node, Print) {
//   auto node2 = Node<std::vector, int>(1);
//   node2.elements = {1, 2, 3, 4, 5};

//   auto fout = std::ofstream("node_print.txt", std::ios::out);
//   fout << node2;
//   EXPECT_EQ(1, 1);
// }

// NOLINTEND(modernize-use-trailing-return-type)
}  // namespace soft_heap::test