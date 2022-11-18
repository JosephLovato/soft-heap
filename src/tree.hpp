#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <memory>
#include <vector>

#include "node.hpp"
#include "policies.hpp"

namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class Tree {
 private:
  using NodePtr = std::unique_ptr<Node<List, Element>>;
  using TreePtr = std::shared_ptr<Tree>;
  using WeakTreePtr = std::weak_ptr<Tree>;

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
        suffix_min() {}

  friend auto operator<<(std::ostream& out, Tree& tree) -> std::ostream& {
    const auto suff = (tree.suffix_min.expired())
                          ? "nullptr"
                          : std::to_string(TreePtr(tree.suffix_min)->rank());

    out << "Tree: " << tree.rank() << "(rank)"
        << "\nsuffix_min: " << suff << "\nwith Nodes:\n";

    const std::function<void(NodePtr&)> preorder = [&](auto& n) {
      if (n == nullptr) {
        return;
      }
      out << *n << '\n';
      preorder(n->left);
      preorder(n->right);
    };
    preorder(tree.root);
    out << std::endl;
    return out;
  }

  constexpr auto rank() noexcept -> int { return root->rank; }

  NodePtr root;            // root of node-based tree
  TreePtr next;            // right
  TreePtr prev;            // left
  WeakTreePtr suffix_min;  // tree with minimum ckey to the right
};

}  // namespace soft_heap