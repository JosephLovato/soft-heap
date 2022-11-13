#pragma once
#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "node.hpp"
#include "policies.hpp"

namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class Tree {
 private:
  using NodePtr = std::unique_ptr<Node<List, Element>>;
  using TreePtr = std::shared_ptr<Tree<List, Element>>;

  constexpr auto MakeNodePtr(Element&& element) {
    return std::make_unique<Node<List, Element>>(
        std::forward<Element>(element));
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