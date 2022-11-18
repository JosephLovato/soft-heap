#pragma once
#include <cmath>
#include <fstream>
#include <memory>

#include "node.hpp"
#include "policies.hpp"
#include "tree.hpp"

namespace soft_heap {

template <template <class... T> class List, policy::TotalOrdered Element>
class SoftHeap {
 private:
  using TreePtr = std::shared_ptr<Tree<List, Element>>;
  using NodePtr = std::unique_ptr<Node<List, Element>>;

  [[nodiscard]] constexpr auto MakeTreePtr(Element&& element) noexcept {
    return std::make_shared<Tree<List, Element>>(
        std::forward<Element>(element));
  }

  [[nodiscard]] constexpr auto MakeNodePtr(NodePtr& x, NodePtr& y,
                                           double r) noexcept {
    return std::make_unique<Node<List, Element>>(std::forward<NodePtr>(x),
                                                 std::forward<NodePtr>(y),
                                                 std::forward<double>(r));
  }

  constexpr void swap(SoftHeap& P) noexcept {
    first_tree.swap(P.first_tree);
    last_tree.swap(P.last_tree);
  }

 public:
  SoftHeap() = delete;

  constexpr explicit SoftHeap(Element element, double eps = 0.1) noexcept
      : first_tree(MakeTreePtr(std::forward<Element>(element))),
        last_tree(first_tree),
        epsilon(eps),
        r(ceil(log2(1 / eps)) + 5) {}

  // Build from STL style iterators
  template <class InputIterator>
  SoftHeap(InputIterator first, InputIterator last, double eps = 0.1) noexcept
      : SoftHeap(*first, eps) {
    std::for_each(std::next(first), last, [&](auto&& e) { Insert(e); });
  }
  ~SoftHeap() = default;

  constexpr void Meld(SoftHeap&& P) noexcept {
    if (P.rank() > rank()) {
      swap(P);  // Just use P.Meld(Q) ?
    }
    MergeInto(std::forward<SoftHeap>(P));
    RepeatedCombine(P.rank());
  }

  // TODO(TEAM) unit test
  void Insert(Element e) { Meld(SoftHeap(std::forward<Element>(e), epsilon)); }

  // TODO(TEAM) unit test
  auto ExtractMin() -> Element {
    assert(first_tree != nullptr);
    auto tree = (first_tree->suffix_min.expired())
                    ? first_tree
                    : std::shared_ptr(first_tree->suffix_min);
    assert(not tree->root->elements.empty());
    const auto e = tree->root->elements.back();
    tree->root->elements.pop_back();
    if (tree->root->elements.size() <= tree->root->size / 2) {
      if (not tree->root->IsLeaf()) {
        tree->root->Sift();
        UpdateSuffixMin(tree);
      } else if (tree->root->elements.empty()) {
        RemoveTree(tree);
        UpdateSuffixMin(last_tree);  // maybe a faster way
      }
    }

    return e;
  }

  // TODO(Team): unit test
  void MergeInto(SoftHeap&& P) {
    if (P.rank() > rank()) {
      return;
    }
    auto tree1 = P.first_tree;
    auto tree2 = first_tree;
    while (tree1 != nullptr) {
      while (tree1->rank() > tree2->rank()) {
        tree2 = tree2->next;
      }
      const auto tree1_temp = tree1->next;
      InsertTree(tree1, tree2);
      tree1 = tree1_temp;
    }
  }

  // TODO(Team): unit test
  void RepeatedCombine(int k) {
    auto tree = first_tree;
    while (tree != nullptr and tree->next != nullptr) {
      if (tree->rank() == tree->next->rank()) {
        if (tree->next->next == nullptr or
            tree->rank() != tree->next->next->rank()) {
          tree->root = std::move(MakeNodePtr(tree->root, tree->next->root, r));
          RemoveTree(tree->next);
          if (tree->next == nullptr) {
            break;
          }
        }
      } else if (tree->rank() > k) {
        break;
      }
      if (tree->rank() != tree->next->rank()) {
        tree = tree->next;
      }
    }
    last_tree = tree;
    UpdateSuffixMin(tree);
  }

  // TODO(TEAM) unit test
  static void UpdateSuffixMin(TreePtr tree) noexcept {
    while (tree != nullptr) {
      if (tree->next != nullptr) {
        auto tree_next_suff_min = (tree->next->suffix_min.expired())
                                      ? tree->next
                                      : std::shared_ptr(tree->next->suffix_min);
        tree->suffix_min = (tree->root->ckey > tree_next_suff_min->root->ckey)
                               ? tree_next_suff_min
                               : tree;
      }
      tree = tree->prev;
    }
  }

  // TODO(TEAM) unit test
  constexpr void InsertTree(TreePtr tree1, TreePtr tree2) noexcept {
    tree1->next = tree2;
    if (tree2->prev == nullptr) {
      first_tree = tree1;
    } else {
      tree2->prev->next = tree1;
      tree1->prev = tree2->prev;  // added by us
    }
    tree2->prev = tree1;  // added by us
  }

  // TODO(TEAM) unit test
  constexpr void RemoveTree(TreePtr tree) noexcept {
    if (tree->prev == nullptr) {
      first_tree = tree->next;
    } else {
      tree->prev->next = tree->next;
    }
    if (tree->next != nullptr) {
      tree->next->prev = tree->prev;
    }
  }

  constexpr auto rank() noexcept -> int { return last_tree->rank(); }

  friend auto operator<<(std::ostream& out, SoftHeap& soft_heap) noexcept
      -> std::ostream& {
    out << "SoftHeap: " << soft_heap.rank() << "(rank) with trees: \n";
    const std::function<void(TreePtr&)> preorder = [&](auto& n) {
      if (n == nullptr) {
        return;
      }
      out << "-------------------\n" << *n << '\n';
      preorder(n->next);
    };
    preorder(soft_heap.first_tree);
    out << std::endl;
    return out;
  }

  TreePtr first_tree;
  TreePtr last_tree;
  const double epsilon;
  const double r;
};

}  // namespace soft_heap