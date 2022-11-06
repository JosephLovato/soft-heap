#pragma once
#include <algorithm>
#include <cmath>
#include <memory>

#include "policies.hpp"
namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class Node : std::enable_shared_from_this<Node<List, Element>> {
 public:
  explicit Node(Element&& element)
      : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
    elements.emplace_back(std::move(element));
  }

  constexpr auto IsLeaf(const Node& n) -> bool {
    return (n.left == nullptr and n.right == nullptr);
  };

  // TODO(Joey): Unit Test sift
  constexpr void Sift(std::shared_ptr<Node> x) {
    while (x.elements < x.size() and not IsLeaf(x)) {
      if (x.left == nullptr or
          (x.right != nullptr and x.left.ckey > x.right.ckey)) {
        std::swap(x.left, x.right);
        std::move(x.left.elements.begin(), x.left.elements.end(),
                  x.elements.end());
        x.key = x.left.key;
        x.left.elements.clear();
        if (IsLeaf(x.left)) {
          x.left = nullptr;
        } else {
          Sift(x.left);
        }
      }
    }
  }

  // r passed in from repeat_combine() from SoftHeap
  Node(std::shared_ptr<Node> x, std::shared_ptr<Node> y, double r) {
    left = x;
    right = y;
    rank = x.rank + 1;
    size = (rank > r) ? ceil((3 * x.size + 1) / 2) : 1;
    Sift(std::enable_shared_from_this<Node>::shared_from_this());
  }

  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;
  int rank;
  int size;
  List<Element> elements;
  Element ckey;  // upper bound for elements
};
}  // namespace soft_heap
