#pragma once
#include <algorithm>
#include <cmath>
#include <compare>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

#include "policies.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element,
          policy::TotalOrderedContainer List = std::vector<Element>>
class Node {
 public:
  using NodePtr = std::unique_ptr<Node>;

  Node() = delete;

  constexpr explicit Node(Element&& element) noexcept
      : elements(List{std::forward<Element>(element)}),
        ckey(std::forward<Element>(element)),
        rank(0),
        size(1),
        left(nullptr),
        right(nullptr) {}

  constexpr explicit Node(int rank, int size, List&& list) noexcept
      : elements(std::move(list)),
        ckey(*std::max_element(elements.begin(), elements.end())),
        rank(rank),
        size(size),
        left(nullptr),
        right(nullptr) {}

  constexpr explicit Node(NodePtr&& node1, NodePtr&& node2,
                          int struct_param) noexcept
      : rank((node1 == nullptr) ? node2->rank + 1 : node1->rank + 1),
        size((rank > struct_param)
                 ? (node1 == nullptr) ? node2->size + 1 : node1->size + 1
                 : 1),
        left(std::move(node1)),
        right(std::move(node2)) {
    Sift();
  }

  [[nodiscard]] constexpr auto IsLeaf() const noexcept {
    return left == nullptr and right == nullptr;
  };

  constexpr void Sift() noexcept {
    while (std::ssize(elements) < size and not IsLeaf()) {
      auto& min_child =
          (left == nullptr or (right != nullptr and *left > *right)) ? right
                                                                     : left;
      if (elements.empty()) {
        elements = std::move(min_child->elements);
      } else {
        std::move(std::make_move_iterator(min_child->elements.begin()),
                  std::make_move_iterator(min_child->elements.end()),
                  std::back_inserter(elements));
        min_child->elements.clear();
      }
      ckey = min_child->ckey;
      if (min_child->IsLeaf()) {
        min_child.reset();  // deallocate child
      } else {
        min_child->Sift();
      }
    }
  }
  // std::swap(left, right);
  // auto& min_child = left;
  // }
  // // std::back_inserter(elements));
  // elements.splice(elements.end(), std::move(min_child->elements));
  // min_child = nullptr;
  // }
  // elements.insert(elements.begin(),
  //                 std::make_move_iterator(min_child->elements.begin()),
  //                 std::make_move_iterator(min_child->elements.end()));
  // elements.reserve(elements.size() + min_child->elements.size());

  [[nodiscard]] constexpr auto back() const noexcept { return elements.back(); }

  constexpr void pop_back() noexcept { elements.pop_back(); }

  constexpr auto operator<=>(const Node& that) const noexcept
      -> std::strong_ordering {
    return this->ckey <=> that.ckey;
  }

  // constexpr auto operator<=>(Node&& that) const noexcept
  //     -> std::strong_ordering {
  //   return this->ckey <=> that.ckey;
  // }

  friend auto operator<<(std::ostream& out, const Node& node) noexcept
      -> std::ostream& {
    out << "Node: " << node.ckey << " (ckey), rank: " << node.rank
        << ", size: " << node.size;
    out << "\nwith elements: ";
    std::copy(node.elements.begin(), node.elements.end(),
              std::ostream_iterator<Element>(out, ", "));
    auto left =
        (node.left == nullptr) ? "nullptr" : std::to_string(node.left->ckey);
    auto right =
        (node.right == nullptr) ? "nullptr" : std::to_string(node.right->ckey);
    out << "\nand children: " << left << ", " << right << std::endl;
    return out;
  }

  List elements;
  Element ckey;
  const int rank;
  const int size;
  NodePtr left;
  NodePtr right;
};

}  // namespace soft_heap

// namespace soft_heap {
// template <template <class... T> class List, policy::TotalOrdered Element>
// class Node {
//  private:
//   using NodePtr = std::unique_ptr<Node>;

//  public:
//   Node() = default;

//   constexpr explicit Node(Element&& element) noexcept
//       : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
//     elements.emplace_back(std::move(element));
//   }

//   constexpr explicit Node(const Element& element) noexcept
//       : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
//     elements.emplace_back(element);
//   }

//   constexpr Node(int rank, int size, const List<Element>& elements,
//                  const NodePtr& left = nullptr,
//                  const NodePtr& right = nullptr) noexcept
//       : left(left),
//         right(right),
//         rank(rank),
//         size(size),
//         ckey(std::max(elements)),
//         elements(elements) {}

//   constexpr Node(int rank, int size, List<Element>&& elements,
//                  NodePtr&& left = nullptr, NodePtr&& right = nullptr)
//                  noexcept
//       : left(std::move(left)),
//         right(std::move(right)),
//         rank(rank),
//         size(size),
//         elements(std::move(elements)),
//         ckey(*std::max_element(this->elements.begin(), this->elements.end()))
//         {}

//   // r passed in from repeat_combine() from SoftHeap
//   constexpr Node(NodePtr& x, NodePtr& y, double r) noexcept
//       : left(std::move(x)),
//         right(std::move(y)),
//         rank(left->rank + 1),
//         size((rank > r) ? ceil(3 * left->size / 2.0) : 1) {
//     Sift();
//   }

//   constexpr Node(NodePtr&& x, NodePtr&& y, double r) noexcept
//       : left(std::move(x)),
//         right(std::move(y)),
//         rank(left->rank + 1),
//         size((rank > r) ? ceil(3 * left->size / 2.0) : 1) {
//     Sift();
//   }

//   [[nodiscard]] constexpr auto IsLeaf() const noexcept -> bool {
//     return (left == nullptr and right == nullptr);
//   };

//   // TODO(Joey): Unit Test sift
//   constexpr void Sift() noexcept {
//     while (elements.size() < size and not IsLeaf()) {
//       if (left == nullptr or (right != nullptr and left->ckey > right->ckey))
//       {
//         left.swap(right);
//       }
//       std::move(left->elements.begin(), left->elements.end(),
//                 std::back_inserter(elements));
//       ckey = left->ckey;
//       left->elements.clear();
//       if (left->IsLeaf()) {
//         left = nullptr;
//       } else {
//         left->Sift();
//       }
//     }
//   }

//   friend auto operator<<(std::ostream& out, const Node& node) noexcept
//       -> std::ostream& {
//     out << "Node: " << node.ckey << " (ckey), rank: " << node.rank
//         << ", size: " << node.size;
//     out << "\nwith elements: ";
//     std::copy(node.elements.begin(), node.elements.end(),
//               std::ostream_iterator<Element>(out, ", "));
//     auto left =
//         (node.left == nullptr) ? "nullptr" : std::to_string(node.left->ckey);
//     auto right =
//         (node.right == nullptr) ? "nullptr" :
//         std::to_string(node.right->ckey);
//     out << "\nand children: " << left << ", " << right << std::endl;
//     return out;
//   }

//   constexpr auto num_corrupted_keys() noexcept {
//     return std::count_if(elements.begin(), elements.end(),
//                          [&](auto&& x) { return x < ckey; });
//   }

//   NodePtr left;
//   NodePtr right;
//   int rank;
//   int size;
//   List<Element> elements;
//   Element ckey;  // upper bound for elements
// };

// }  // namespace soft_heap
