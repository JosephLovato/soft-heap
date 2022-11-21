#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <type_traits>

#include "node.hpp"
#include "policies.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element,
          policy::TotalOrderedContainer List = std::list<Element>>
class Tree {
 public:
  using NodePtr = std::unique_ptr<Node<Element, List>>;
  using TreeList = std::list<Tree<Element, List>>;
  using TreeListIt = typename TreeList::iterator;

  [[nodiscard]] constexpr auto MakeNodePtr(Element&& elem) noexcept {
    return std::make_unique<Node<Element, List>>(std::forward<Element>(elem));
  }

  constexpr explicit Tree(Element&& element) noexcept
      : root(MakeNodePtr(std::forward<Element>(element))) {}

  [[nodiscard]] constexpr auto rank() const noexcept { return root->rank; }

  friend auto operator<<(std::ostream& out, Tree& tree) noexcept
      -> std::ostream& {
    out << "Tree: " << tree.rank() << "(rank)"
        << "\nwith Nodes:\n";
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

  NodePtr root;
  TreeListIt min_ckey;

 private:
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
// // class Tree {
// //  private:
// //   using NodePtr = std::unique_ptr<Node<List, Element>>;
// //   using TreePtr = std::shared_ptr<Tree>;
// //   using WeakTreePtr = std::weak_ptr<Tree>;

// //  public:
// //   constexpr explicit Tree(Element&&) noexcept;
// //   constexpr auto rank() const noexcept;

// //   NodePtr root;
// //   TreePtr next;
// //   TreePtr prev;
// //   WeakTreePtr suffix_min;
// // };

// namespace soft_heap {
// template <template <class... T> class List, policy::TotalOrdered Element>
// class Tree {
//  private:
//   using NodePtr = std::unique_ptr<Node<List, Element>>;
//   using TreePtr = std::shared_ptr<Tree>;
//   using WeakTreePtr = std::weak_ptr<Tree>;

//   constexpr auto MakeNodePtr(Element&& element) noexcept {
//     return std::make_unique<Node<List, Element>>(
//         std::forward<Element>(element));
//   }

//  public:
//   Tree() = delete;
//   constexpr explicit Tree(Element&& element) noexcept
//       : root(MakeNodePtr(std::forward<Element>(element))),
//         next(nullptr),
//         prev(nullptr),
//         suffix_min() {}

//   [[nodiscard]] constexpr auto rank() const noexcept -> int {
//     return root->rank;
//   }

//   friend auto operator<<(std::ostream& out, Tree& tree) noexcept
//       -> std::ostream& {
//     const auto suff = (tree.suffix_min.expired())
//                           ? "nullptr"
//                           :
//                           std::to_string(TreePtr(tree.suffix_min)->rank());

//     out << "Tree: " << tree.rank() << "(rank)"
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
//   TreePtr next;            // right
//   TreePtr prev;            // left
//   WeakTreePtr suffix_min;  // tree with minimum ckey to the right
// };

// }  // namespace soft_heap