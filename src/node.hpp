#pragma once
#include <cmath>
#include <compare>
#include <iostream>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

#include "policies.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element,
          policy::TotalOrderedContainer List = std::list<Element>>
class Node {
 public:
  using NodePtr = std::unique_ptr<Node>;

  Node() = delete;

  constexpr explicit Node(Element&& element) noexcept
      : left(nullptr),
        right(nullptr),
        elements(List{std::forward<Element>(element)}),
        ckey(std::forward<Element>(element)),
        rank(0),
        size(1) {}

  constexpr explicit Node(NodePtr&& node1, NodePtr&& node2,
                          int struct_param) noexcept
      : left(std::forward<NodePtr>(node1)),
        right(std::forward<NodePtr>(node2)),
        rank(left->rank + 1),
        size((rank > struct_param) ? left->size + 1 : 1) {
    Sift();
  }

  [[nodiscard]] constexpr auto IsLeaf() const noexcept {
    return left == nullptr and right == nullptr;
  };

  constexpr void Sift() noexcept {
    while (elements.size() < size and not IsLeaf()) {
      auto& min_child = left == nullptr    ? right
                        : right == nullptr ? left
                        : left > right     ? right
                                           : left;  // see <==> overload
      elements.splice(elements.end(), min_child->elements);
      // std::move(min_child->elements.begin(), min_child->elements.end(),
      //           std::back_inserter(elements));
      ckey = min_child->ckey;
      if (min_child->IsLeaf()) {
        min_child = nullptr;  // deallocated child
      } else {
        min_child->Sift();
      }
    }
  }

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

 private:
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
