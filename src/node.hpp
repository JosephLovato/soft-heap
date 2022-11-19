#pragma once
#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>

#include "policies.hpp"

// OUTLINE
// template <template <class... T> class List, std::totally_ordered Element>
// class Node {
//  private:
//   using NodePtr = std::unique_ptr<Node>;

//  public:
//   constexpr explicit Node(const Element&) noexcept;
//   constexpr Node(NodePtr&& x, NodePtr&& y, double r) noexcept;
//   constexpr void Sift() noexcept;

//   NodePtr left;
//   NodePtr right;
//   int rank;
//   int size;
//   List<Element> elements;
//   Element ckey;
// };

namespace soft_heap {
template <template <class... T> class List, policy::TotalOrdered Element>
class Node {
 private:
  using NodePtr = std::unique_ptr<Node>;

 public:
  Node() = default;

  constexpr explicit Node(Element&& element) noexcept
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(std::move(element));
  }

  constexpr explicit Node(const Element& element) noexcept
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(element);
  }

  constexpr Node(int rank, int size, const List<Element>& elements,
                 const NodePtr& left = nullptr,
                 const NodePtr& right = nullptr) noexcept
      : left(left),
        right(right),
        rank(rank),
        size(size),
        ckey(std::max(elements)),
        elements(elements) {}

  constexpr Node(int rank, int size, List<Element>&& elements,
                 NodePtr&& left = nullptr, NodePtr&& right = nullptr) noexcept
      : left(std::move(left)),
        right(std::move(right)),
        rank(rank),
        size(size),
        elements(std::move(elements)),
        ckey(*std::max_element(this->elements.begin(), this->elements.end())) {}

  // r passed in from repeat_combine() from SoftHeap
  constexpr Node(NodePtr& x, NodePtr& y, double r) noexcept
      : left(std::move(x)),
        right(std::move(y)),
        rank(left->rank + 1),
        size((rank > r) ? ceil(3 * left->size / 2.0) : 1) {
    Sift();
  }

  constexpr Node(NodePtr&& x, NodePtr&& y, double r) noexcept
      : left(std::move(x)),
        right(std::move(y)),
        rank(left->rank + 1),
        size((rank > r) ? ceil(3 * left->size / 2.0) : 1) {
    Sift();
  }

  [[nodiscard]] constexpr auto IsLeaf() const noexcept -> bool {
    return (left == nullptr and right == nullptr);
  };

  // TODO(Joey): Unit Test sift
  constexpr void Sift() noexcept {
    while (elements.size() < size and not IsLeaf()) {
      if (left == nullptr or (right != nullptr and left->ckey > right->ckey)) {
        left.swap(right);
      }
      std::move(left->elements.begin(), left->elements.end(),
                std::back_inserter(elements));
      ckey = left->ckey;
      left->elements.clear();
      if (left->IsLeaf()) {
        left = nullptr;
      } else {
        left->Sift();
      }
    }
  }

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

  constexpr auto num_corrupted_keys() noexcept {
    return std::count_if(elements.begin(), elements.end(),
                         [&](auto&& x) { return x < ckey; });
  }

  NodePtr left;
  NodePtr right;
  int rank;
  int size;
  List<Element> elements;
  Element ckey;  // upper bound for elements
};

}  // namespace soft_heap