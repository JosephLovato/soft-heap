#pragma once
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <type_traits>

#include "flat_node.hpp"
#include "policies.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element, policy::TotalOrderedContainer List,
          int inverse_epsilon>
class FlatTree {
 public:
  using TreeList = std::list<FlatTree>;
  using TreeListIt = typename TreeList::iterator;

  constexpr explicit FlatTree(Element&& element) noexcept {
    node_heap.emplace_back(std::forward<Element>(element));
  }

  [[nodiscard]] constexpr auto rank() const noexcept {
    // return node_heap.back()->rank;
    return static_cast<int>(std::log2(std::ssize(node_heap)));
  }

  constexpr void Sift() noexcept {
    constexpr auto IsLeaf = [](int idx, auto& node_heap) constexpr {
      return std::ssize(node_heap) < 2 * idx + 2 or
             (node_heap[2 * idx + 1].ckey == -1 and
              (std::ssize(node_heap) < 2 * idx + 3 or
               node_heap[2 * idx + 2].ckey == -1));
    };
    constexpr auto RecursiveSift = [](int idx, auto&& node_heap, auto&& self,
                                      auto&& IsLeaf) constexpr -> void {
      auto& elements = node_heap[idx].elements;

      while (std::ssize(elements) < node_heap[idx].size and
             not IsLeaf(idx, node_heap)) {
        const auto left_idx = 2 * idx + 1;
        const auto right_idx = 2 * idx + 2;
        const auto min_child_idx =
            (std::ssize(node_heap) < right_idx or
             node_heap[left_idx].ckey == -1) or
                    (std::ssize(node_heap) > right_idx and
                     node_heap[right_idx].ckey != -1 and
                     node_heap[left_idx] > node_heap[right_idx])
                ? right_idx
                : left_idx;
        auto& min_elements = node_heap[min_child_idx].elements;
        if (elements.empty()) {
          elements = std::move(min_elements);
        } else {
          elements.insert(elements.end(),
                          std::make_move_iterator(min_elements.begin()),
                          std::make_move_iterator(min_elements.end()));
        }
        node_heap[idx].ckey = node_heap[min_child_idx].ckey;
        if (IsLeaf(min_child_idx, node_heap)) {
          node_heap[min_child_idx].ckey = -1;  // mark for removal
        } else {
          min_elements.clear();
          self(min_child_idx, node_heap, self, IsLeaf);
        }
      }
    };
    RecursiveSift(0, node_heap, RecursiveSift, IsLeaf);

    std::make_heap(node_heap.begin(), node_heap.end(), std::greater<>());
    while (node_heap[0].ckey == -1) {
      std::pop_heap(node_heap.begin(), node_heap.end(), std::greater<>());
      node_heap.pop_back();
    }
  }

  constexpr auto operator<=>(const FlatTree& that) const noexcept
      -> std::strong_ordering {
    return this->rank() <=> that.rank();
  }

  constexpr auto operator<=>(FlatTree&& that) const noexcept
      -> std::strong_ordering {
    return this->rank() <=> that.rank();
  }

  constexpr auto operator<=>(int that) const noexcept -> std::strong_ordering {
    return this->rank() <=> that;
  }

  friend auto operator<<(std::ostream& out, FlatTree& tree) noexcept
      -> std::ostream& {
    out << "Tree: " << tree.rank() << "(rank)"
        << "\nwith Nodes:\n";

    auto preorder = [&](auto self, int n) -> void {
      out << tree.node_heap[n];
      auto left = (std::ssize(tree.node_heap) > 2 * n + 1)
                      ? tree.node_heap[2 * n + 1].ckey
                      : -1;
      auto right = (std::ssize(tree.node_heap) > 2 * n + 2)
                       ? tree.node_heap[2 * n + 2].ckey
                       : -1;
      out << "\nand children: " << left << ", " << right << '\n' << std::endl;
      if (std::ssize(tree.node_heap) > 2 * n + 1) {
        self(self, 2 * n + 1);
        // preorder(2 * n + 1, preorder);
      }
      if (std::ssize(tree.node_heap) > 2 * n + 2) {
        self(self, 2 * n + 2);
        // preorder(2 * n + 2, preorder);
      }
    };
    preorder(preorder, 0);
    out << std::endl;
    return out;
  }

  std::vector<FlatNode<int, List, inverse_epsilon>> node_heap;
  TreeListIt min_ckey;
};

}  // namespace soft_heap

// #pragma once
// #include <algorithm>
// #include <cmath>
// #include <functional>
// #include <iomanip>
// #include <memory>
// #include <vector>

// #include "node.hpp"
// #include "policies.hpp"

// // OUTLINE
// // template <template <class... T> class List, std::totally_ordered
// Element>
// // class FlatTree {
// //  private:
// //   using NodePtr = std::unique_ptr<Node<List, Element>>;
// //   using FlatTreePtr = std::shared_ptr<FlatTree>;
// //   using WeakFlatTreePtr = std::weak_ptr<FlatTree>;

// //  public:
// //   constexpr explicit FlatTree(Element&&) noexcept;
// //   constexpr auto rank() const noexcept;

// //   NodePtr root;
// //   FlatTreePtr next;
// //   FlatTreePtr prev;
// //   WeakFlatTreePtr suffix_min;
// // };

// namespace soft_heap {
// template <template <class... T> class List, policy::TotalOrdered Element>
// class FlatTree {
//  private:
//   using NodePtr = std::unique_ptr<Node<List, Element>>;
//   using FlatTreePtr = std::shared_ptr<FlatTree>;
//   using WeakFlatTreePtr = std::weak_ptr<FlatTree>;

//   constexpr auto MakeNodePtr(Element&& element) noexcept {
//     return std::make_unique<Node<List, Element>>(
//         std::forward<Element>(element));
//   }

//  public:
//   FlatTree() = delete;
//   constexpr explicit FlatTree(Element&& element) noexcept
//       : root(MakeNodePtr(std::forward<Element>(element))),
//         next(nullptr),
//         prev(nullptr),
//         suffix_min() {}

//   [[nodiscard]] constexpr auto rank() const noexcept -> int {
//     return root->rank;
//   }

//   friend auto operator<<(std::ostream& out, FlatTree& tree) noexcept
//       -> std::ostream& {
//     const auto suff = (tree.suffix_min.expired())
//                           ? "nullptr"
//                           :
//                           std::to_string(FlatTreePtr(tree.suffix_min)->rank());

//     out << "FlatTree: " << tree.rank() << "(rank)"
//         << "\nsuffix_min: " << suff << "\nwith Nodes:\n";

//     const std::function<void(NodePtr&)> preorder = [&](auto& n) {
//       if (n == nullptr) {
//         return;
//       }
//       out << *n << '\n';
//       preorder(n->left);
//       preorder(n->right);
//     };
//     preorder(tree.root);
//     out << std::endl;
//     return out;
//   }

//   constexpr auto num_corrupted_keys() noexcept {
//     int num = 0;
//     const std::function<void(NodePtr&)> preorder = [&](auto& n) {
//       if (n == nullptr) {
//         return;
//       }
//       num += n->num_corrupted_keys();
//       preorder(n->left);
//       preorder(n->right);
//     };
//     preorder(root);
//     return num;
//   }

//   NodePtr root;            // root of node-based tree
//   FlatTreePtr next;            // right
//   FlatTreePtr prev;            // left
//   WeakFlatTreePtr suffix_min;  // tree with minimum ckey to the right
// };

// }  // namespace soft_heap