#pragma once
#include <algorithm>
#include <cmath>
#include <memory>

#include "policies.hpp"

namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class Node {
  // class Node : std::enable_shared_from_this<Node<List, Element>> {
 private:
  using NodePtr = std::unique_ptr<Node>;

 public:
  explicit Node(Element&& element)
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(std::move(element));
  }

  constexpr auto IsLeaf() -> bool {
    return (left == nullptr and right == nullptr);
  };

  // TODO(Joey): Unit Test sift
  constexpr void Sift() {
    while (elements.size() < size and not IsLeaf()) {
      if (left == nullptr or (right != nullptr and left->ckey > right->ckey)) {
        std::swap(left, right);
        std::move(left->elements.begin(), left->elements.end(), elements.end());
        ckey = left->ckey;
        left->elements.clear();
        if (left->IsLeaf()) {
          left = nullptr;
        } else {
          left->Sift();
        }
      }
    }
  }

  // r passed in from repeat_combine() from SoftHeap
  Node(NodePtr&& x, NodePtr&& y, double r) {
    left = std::move(x);
    right = std::move(y);
    rank = x->rank + 1;
    size = (rank > r) ? ceil((3 * x->size + 1) / 2) : 1;
    // Sift(std::enable_shared_from_this<Node>::shared_from_this());
    Sift();
  }

  NodePtr left;
  NodePtr right;
  int rank;
  int size;
  List<Element> elements;
  Element ckey;  // upper bound for elements
};

}  // namespace soft_heap
