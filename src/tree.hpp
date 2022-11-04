#pragma once
#include <algorithm>
#include <cmath>
#include <memory>

#include "policies.hpp"
namespace soft_heap {

using policy::Arithmetic;

template <template <Arithmetic...> class List, Arithmetic Element>
class Tree {
 private:
  class Node {
   public:
    explicit Node(Element&& element)
        : left(nullptr), right(nullptr), rank(0), size(1), ckey(element) {
      elements.emplace_back(std::move(element));
    }

    constexpr IsLeaf(const Node& n) {
      return (n.left == nullptr and n.right == nullptr);
    };

    // TODO: Unit Test sift
    constexpr void sift() {
      while (elements < x.size() and not IsLeaf(this)) {
        if (left == nullptr or (right != nullptr and left.ckey > right.ckey)) {
          std::swap(left, right);
          std::move(left.list.begin(), left.list.end(), list.end());
          key = left.key;
          left.list.clear();
          if (IsLeaf(left)) {
            left = nullptr;
          } else {
            sift(left);
          }
        }
      }
    }

    constexpr void sift(std::shared_ptr<Node> x) {
      while (x.elements < x.size() and not IsLeaf(x)) {
        if (x.left == nullptr or
            (x.right != nullptr and x.left.ckey > x.right.ckey)) {
          std::swap(x.left, x.right);
          std::move(x.left.list.begin(), x.left.list.end(), x.list.end());
          x.key = x.left.key;
          x.left.list.clear();
          if (IsLeaf(x.left)) {
            x.left = nullptr;
          } else {
            sift(x.left);
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
      sift();
    }

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    int rank;
    int size;
    List<Element> elements;
    Element ckey;  // upper bound for elements
  };

  // "Typedefs"
  using NodePtr = std::shared_ptr<Node>;
  using TreePtr = std::shared_ptr<Tree<List, Element>>;
  constexpr auto MakeNodePtr(Element&& element) {
    return std::make_shared<Node>(std::forward<Element>(element));
  }

 public:
  Tree() = delete;
  explicit Tree(Element&& element)
      : root(MakeNodePtr(std::forward<Element>(element))),
        next(nullptr),
        prev(nullptr),
        suffix_min(nullptr),
        rank(0) {}
  NodePtr root;        // root of node-based tree
  TreePtr next;        // right
  TreePtr prev;        // left
  TreePtr suffix_min;  // tree with minimum ckey to the right
  int rank;
};

}  // namespace soft_heap