#pragma once
#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>

#include "policies.hpp"

namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class Node {
 private:
  using NodePtr = std::unique_ptr<Node>;

 public:
  Node() = default;

  explicit Node(Element&& element)
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(std::move(element));
  }

  explicit Node(const Element& element)
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(element);
  }

  explicit Node(int rank, int size, const List<Element>& elements,
                const NodePtr& left = nullptr, const NodePtr& right = nullptr)
      : left(left),
        right(right),
        rank(rank),
        size(size),
        ckey(std::max(elements)),
        elements(elements) {}

  explicit Node(int rank, int size, List<Element>&& elements,
                NodePtr&& left = nullptr, NodePtr&& right = nullptr)
      : left(std::move(left)),
        right(std::move(right)),
        rank(rank),
        size(size),
        elements(std::move(elements)),
        ckey(*std::max_element(this->elements.begin(), this->elements.end())) {}

  // r passed in from repeat_combine() from SoftHeap
  constexpr Node(NodePtr& x, NodePtr& y, double r)
      : left(std::move(x)),
        right(std::move(y)),
        rank(left->rank + 1),
        size((rank > r) ? ceil(3 * left->size / 2.0) : 1) {
    Sift();
  }

  constexpr auto IsLeaf() -> bool {
    return (left == nullptr and right == nullptr);
  };

  // TODO(Joey): Unit Test sift
  constexpr void Sift() {
    while (elements.size() < size and not IsLeaf()) {
      if (left == nullptr or (right != nullptr and left->ckey > right->ckey)) {
        std::swap(left, right);
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

  NodePtr left;
  NodePtr right;
  int rank;
  int size;
  List<Element> elements;
  Element ckey;  // upper bound for elements
};

}  // namespace soft_heap
