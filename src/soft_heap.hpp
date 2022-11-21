#pragma once
#include <__iterator/concepts.h>

#include <cmath>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <type_traits>

#include "policies.hpp"
#include "tree.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element,
          policy::TotalOrderedContainer List = std::list<Element>>
class SoftHeap {
 public:
  using NodePtr = std::unique_ptr<Node<Element, List>>;
  using TreeList = std::list<Tree<Element, List>>;
  using TreeListIt = typename TreeList::iterator;

  constexpr explicit SoftHeap(Element element, double eps = 0.1) noexcept
      : epsilon(eps), struct_param(ConstCeil(std::log2(1.0 / eps)) + 5) {
    trees.emplace_back(std::forward<Element>(element));
    trees.front().min_ckey = trees.begin();
  }

  constexpr SoftHeap(std::input_iterator auto first,
                     std::input_iterator auto last, double eps = 0.1) noexcept
      : SoftHeap(*first, eps) {
    std::for_each(std::next(first), last, [&](auto&& e) { Insert(e); });
  }

  constexpr void Insert(Element e) noexcept {
    Meld(SoftHeap(std::forward<Element>(e), epsilon));
  }

  constexpr void Meld(SoftHeap&& P) noexcept {
    if (P.rank() > rank()) {
      trees.swap(P.trees);
    }
    const auto p_rank = P.rank();
    trees.merge(P.trees,
                [](auto&& a, auto&& b) { return a.rank() <= b.rank(); });
    auto temp = trees.begin();
    for (auto tree = std::next(trees.begin()); tree != trees.end();
         std::advance(tree, 1)) {
      if (tree->rank() == temp->rank()) {
        tree->root = MakeNodePtr(std::forward<NodePtr>(tree->root),
                                 std::forward<NodePtr>(temp->root));
        trees.erase(temp);
      }
      std::advance(temp, 1);
      if (tree->rank() >= p_rank) {
        UpdateSuffixMin(tree);
        return;
      }
    }
  }

  [[nodiscard]] constexpr auto ExtractMin() noexcept {
    const auto& min_tree = trees.front().min_ckey;
    const auto& x = min_tree->root;
    const auto first_elem = x->back();
    x->pop_back();
    if (2 * x->elements.size() < x->size) {
      if (not x->IsLeaf()) {
        x->Sift();
        UpdateSuffixMin(min_tree);
      } else if (x->elements.empty()) {
        if (min_tree != trees.begin()) {
          const auto prev = std::prev(min_tree);
          trees.erase(min_tree);
          UpdateSuffixMin(prev);
        } else {
          trees.erase(min_tree);
        }
      }
    }
    return first_elem;
  }

  friend auto operator<<(std::ostream& out, SoftHeap& soft_heap) noexcept
      -> std::ostream& {
    out << "SoftHeap: " << soft_heap.rank() << "(rank) with trees: \n";
    for (auto&& x : soft_heap.trees) {
      out << "-------------------\n" << x << '\n';
    }
    out << std::endl;
    return out;
  }

  TreeList trees;

 private:
  double epsilon;
  int struct_param;

  [[nodiscard]] constexpr auto MakeNodePtr(NodePtr&& x, NodePtr&& y) noexcept {
    return std::make_unique<Node<Element, List>>(
        std::forward<NodePtr>(x), std::forward<NodePtr>(y), struct_param);
  }

  [[nodiscard]] constexpr auto rank() const noexcept {
    return trees.back().rank();
  }

  constexpr void Combine(TreeListIt tree1, TreeListIt tree2) noexcept {
    tree1->root = MakeNodePtr(std::forward<NodePtr>(tree1->root),
                              std::forward<NodePtr>(tree2->root));
    trees.erase(tree2);
  }

  constexpr void UpdateSuffixMin(TreeListIt it) noexcept {
    std::advance(it, 1);
    while (it != trees.begin()) {
      std::advance(it, -1);
      it->min_ckey = (std::next(it) == trees.end() or
                      std::next(it)->min_ckey->root->ckey >= it->root->ckey)
                         ? it
                         : std::next(it)->min_ckey;
    }
  }
};

}  // namespace soft_heap

// constexpr auto ExtractMin() noexcept {
//   auto first_min_ckey =
//       first->min_ckey == nullptr ? first : TreePtr(first->min_ckey);
//   auto& x = first_min_ckey->root;
//   auto first_elem = x->elements.back();
//   x->elements.pop_back();
//   if (2 * x->elements.size() < x->size) {
//     if (not x->IsLeaf()) {
//       x->Sift();
//       first_min_ckey->UpdateSuffixMin();
//     } else if (x->elements.empty()) {
//       if (first_min_ckey->prev != nullptr) {
//         first_min_ckey->prev->UpdateSuffixMin();
//         if (first_min_ckey->next == nullptr) {
//           rank = first_min_ckey->prev->rank;
//         }
//       } else if (first_min_ckey->next == nullptr) {
//         first = nullptr;
//       }
//     }
//   }
//   return first_elem;
// }

// constexpr auto ExtractMin() noexcept {
//   auto min_tree = trees.front().min_ckey;
//   auto elem = min_tree->root->elements.back();
//   min_tree->root->elements.pop_back();
//   if (2 * min_tree->root->elements.size() < min_tree->root.size()) {
//     min_tree->root->Sift();
//     UpdateSuffixMin(auto& it)
//   }
// }
//
// #pragma once
// #include <cmath>
// #include <fstream>
// #include <memory>

// #include "node.hpp"
// #include "policies.hpp"
// #include "tree.hpp"

// namespace soft_heap {

// // OUTLINE
// // template <template <class... T> class List, std::totally_ordered Element>
// // class SoftHeap {
// //  private:
// //   using TreePtr = std::shared_ptr<Tree<List, Element>>;
// //   using NodePtr = std::unique_ptr<Node<List, Element>>;

// //   constexpr void MergeInto(SoftHeap&&) noexcept;
// //   constexpr void RepeatedCombine(int) noexcept;
// //   constexpr static void UpdateSuffixMin(TreePtr) noexcept;
// //   constexpr void InsertTree(TreePtr, TreePtr) noexcept;
// //   constexpr void RemoveTree(TreePtr) noexcept;
// //   constexpr auto rank() const noexcept -> int;
// //   template <class InputIterator>
// //   constexpr SoftHeap(InputIterator, InputIterator, double);

// //  public:
// //   constexpr explicit SoftHeap(Element, double) noexcept;
// //   constexpr void Meld(SoftHeap&& P) noexcept;
// //   constexpr void Insert(Element e) noexcept;
// //   constexpr auto ExtractMin() noexcept -> Element;

// //   TreePtr first_tree;
// //   TreePtr last_tree;
// //   const double epsilon;
// //   const double r;
// // };

// template <template <class... T> class List, policy::TotalOrdered Element>
// class SoftHeap {
//  private:
//   using TreePtr = std::shared_ptr<Tree<List, Element>>;
//   using NodePtr = std::unique_ptr<Node<List, Element>>;

//   [[nodiscard]] constexpr auto MakeTreePtr(Element&& element) noexcept {
//     return std::make_shared<Tree<List, Element>>(
//         std::forward<Element>(element));
//   }

//   [[nodiscard]] constexpr auto MakeNodePtr(NodePtr& x, NodePtr& y,
//                                            double r) noexcept {
//     return std::make_unique<Node<List, Element>>(std::forward<NodePtr>(x),
//                                                  std::forward<NodePtr>(y),
//                                                  std::forward<double>(r));
//   }

//   // TODO(Team): unit test
//   constexpr void MergeInto(SoftHeap&& P) noexcept {
//     if (P.rank() > rank()) {
//       return;
//     }
//     auto tree1 = P.first_tree;
//     auto tree2 = first_tree;
//     while (tree1 != nullptr) {
//       while (tree1->rank() > tree2->rank()) {
//         tree2 = tree2->next;
//       }
//       const auto tree1_temp = tree1->next;
//       InsertTree(tree1, tree2);
//       tree1 = tree1_temp;
//     }
//   }

//   // TODO(Team): unit test
//   constexpr void RepeatedCombine(int k) noexcept {
//     auto tree = first_tree;
//     while (tree != nullptr and tree->next != nullptr) {
//       if (tree->rank() == tree->next->rank()) {
//         if (tree->next->next == nullptr or
//             tree->rank() != tree->next->next->rank()) {
//           tree->root = std::move(MakeNodePtr(tree->root, tree->next->root,
//           r)); RemoveTree(tree->next); if (tree->next == nullptr) {
//             break;
//           }
//         }
//       } else if (tree->rank() > k) {
//         break;
//       }
//       if (tree->rank() != tree->next->rank()) {
//         tree = tree->next;
//       }
//     }
//     last_tree = tree;
//     UpdateSuffixMin(tree);
//   }

//   // TODO(TEAM) unit test
//   constexpr static void UpdateSuffixMin(TreePtr tree) noexcept {
//     while (tree != nullptr) {
//       if (tree->next != nullptr) {
//         const auto tree_next_suff_min =
//             (tree->next->suffix_min.expired())
//                 ? tree->next
//                 : std::shared_ptr(tree->next->suffix_min);
//         tree->suffix_min = (tree->root->ckey >
//         tree_next_suff_min->root->ckey)
//                                ? tree_next_suff_min
//                                : tree;
//       }
//       tree = tree->prev;
//     }
//   }

//   // TODO(TEAM) unit test
//   constexpr void InsertTree(TreePtr tree1, TreePtr tree2) noexcept {
//     tree1->next = tree2;
//     if (tree2->prev == nullptr) {
//       first_tree = tree1;
//     } else {
//       tree2->prev->next = tree1;
//       tree1->prev = tree2->prev;  // added by us
//     }
//     tree2->prev = tree1;  // added by us
//   }

//   // TODO(TEAM) unit test
//   constexpr void RemoveTree(TreePtr tree) noexcept {
//     if (tree->prev == nullptr) {
//       first_tree = tree->next;
//     } else {
//       tree->prev->next = tree->next;
//     }
//     if (tree->next != nullptr) {
//       tree->next->prev = tree->prev;
//       return;
//     }
//     last_tree = tree->prev;
//   }

//   [[nodiscard]] constexpr auto rank() const noexcept -> int {
//     return last_tree->rank();
//   }

//  public:
//   SoftHeap() = delete;

//   constexpr explicit SoftHeap(Element element, double eps = 0.1) noexcept
//       : first_tree(MakeTreePtr(std::forward<Element>(element))),
//         last_tree(first_tree),
//         epsilon(eps),
//         r(ceil(log2(1 / eps)) + 5) {}

//   // Build from STL style iterators
//   template <class InputIterator>
//   constexpr SoftHeap(InputIterator first, InputIterator last,
//                      double eps = 0.1) noexcept
//       : SoftHeap(*first, eps) {
//     std::for_each(std::next(first), last, [&](auto&& e) { Insert(e); });
//   }
//   ~SoftHeap() = default;

//   constexpr void Meld(SoftHeap&& P) noexcept {
//     if (P.rank() > rank()) {
//       swap(std::forward<SoftHeap>(P));
//     }
//     MergeInto(std::forward<SoftHeap>(P));
//     RepeatedCombine(P.rank());
//   }

//   // TODO(TEAM) unit test
//   constexpr void Insert(Element e) noexcept {
//     Meld(SoftHeap(std::forward<Element>(e), epsilon));
//   }

//   // TODO(TEAM) unit test
//   [[nodiscard]] constexpr auto ExtractMin() noexcept -> Element {
//     assert(first_tree != nullptr);
//     const auto tree = (first_tree->suffix_min.expired())
//                           ? first_tree
//                           : std::shared_ptr(first_tree->suffix_min);
//     assert(not tree->root->elements.empty());
//     const auto e = tree->root->elements.back();
//     tree->root->elements.pop_back();
//     if (tree->root->elements.size() <= tree->root->size / 2) {
//       if (not tree->root->IsLeaf()) {
//         tree->root->Sift();
//         UpdateSuffixMin(tree);
//       } else if (tree->root->elements.empty()) {
//         RemoveTree(tree);
//         UpdateSuffixMin(last_tree);  // maybe a faster way
//       }
//     }

//     return e;
//   }

//   friend auto operator<<(std::ostream& out, SoftHeap& soft_heap) noexcept
//       -> std::ostream& {
//     out << "SoftHeap: " << soft_heap.rank() << "(rank) with trees: \n";
//     const std::function<void(TreePtr&)> preorder = [&](auto& n) {
//       if (n == nullptr) {
//         return;
//       }
//       out << "-------------------\n" << *n << '\n';
//       preorder(n->next);
//     };
//     preorder(soft_heap.first_tree);
//     out << std::endl;
//     return out;
//   }

//   [[nodiscard]] constexpr auto num_corrupted_keys() noexcept {
//     int num = 0;
//     auto tree = first_tree;
//     while (tree != nullptr) {
//       num += tree->num_corrupted_keys();
//       tree = tree->next;
//     }
//     return num;
//   }

//   TreePtr first_tree;
//   TreePtr last_tree;
//   const double epsilon;
//   const double r;
// };

// }  // namespace soft_heap