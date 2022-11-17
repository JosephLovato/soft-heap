#pragma once
#include <cmath>
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
  constexpr auto MakeTreePtr(Element&& element) {
    return std::make_shared<Tree<List, Element>>(
        std::forward<Element>(element));
  }
  constexpr auto MakeNodePtr(NodePtr& x, NodePtr& y, double r) {
    return std::make_unique<Node<List, Element>>(std::forward<NodePtr>(x),
                                                 std::forward<NodePtr>(y),
                                                 std::forward<double>(r));
  }

  constexpr void swap(SoftHeap& P) {
    std::swap(first_tree, P.first_tree);
    std::swap(last_tree, P.last_tree);
    std::swap(rank, P.rank);
  }

 public:
  SoftHeap() = delete;
  explicit SoftHeap(Element element, double eps = 0.1)
      : first_tree(MakeTreePtr(std::forward<Element>(element))),
        last_tree(first_tree),
        rank(0),
        epsilon(eps),
        r(ceil(log2(1 / eps)) + 5) {}

  // Build from STL style iterators
  template <class InputIterator>
  SoftHeap(InputIterator first, InputIterator last, double eps = 0.1)
      : SoftHeap(*first, eps) {
    for (auto it = std::next(first); it != last; ++it) {
      Insert(*it);
    }
  }
  ~SoftHeap() = default;

  // TODO(Team)
  void Meld(SoftHeap&& Q) {
    if (rank > Q.rank) {
      swap(Q);
    }
    MergeInto(Q);
    Q.RepeatedCombine(rank);
    std::swap(first_tree, Q.first_tree);
    std::swap(last_tree, Q.last_tree);
    std::swap(rank, Q.rank);
  }

  // TODO(TEAM) unit test
  void Insert(Element e) { Meld(SoftHeap(std::forward<Element>(e), epsilon)); }

  // TODO(TEAM) unit test
  auto ExtractMin() -> Element {
    assert(first_tree != nullptr);
    auto tree = first_tree->suffix_min;
    auto e = tree->root->elements.back();
    tree->root->elements.pop_back();
    if (tree->root->elements.size() <= tree->root->size / 2) {
      if (not tree->root->IsLeaf()) {
        tree->root->Sift();
        UpdateSuffixMin(tree);
      } else if (tree->root->elements.size() == 0) {
        RemoveTree(tree);
      }
    }
    return e;
  }

  // TODO(Team): unit test
  void MergeInto(SoftHeap& Q) {
    if (rank > Q.rank) {
      return;
    }
    auto tree1 = first_tree;
    auto tree2 = Q.first_tree;
    while (tree1 != nullptr) {
      while (tree1->rank > tree2->rank) {
        tree2 = tree2->next;
      }
      auto tree1_temp = tree1->next;
      Q.InsertTree(tree1, tree2);
      tree1 = tree1_temp;
    }
  }

  // TODO(Team): unit test
  void RepeatedCombine(int k) {
    auto tree = first_tree;
    while (tree->next != nullptr) {
      if (tree->rank == tree->next->rank) {
        if (tree->next->next == nullptr or
            tree->rank != tree->next->next->rank) {
          tree->root = std::move(MakeNodePtr(tree->root, tree->next->root, r));
          tree->rank = tree->root->rank;
          RemoveTree(tree->next);
        }
      } else if (tree->rank > k) {
        break;
      }
      tree = tree->next;
    }
    if (tree->rank > rank) {
      rank = tree->rank;
    }
    UpdateSuffixMin(tree);
  }

  // TODO(TEAM) unit test
  static void UpdateSuffixMin(TreePtr& tree) {
    while (tree != nullptr) {
      if (tree->next != nullptr) {
        const auto tree_next_suff_min = (tree->next->suffix_min == nullptr)
                                            ? tree->next
                                            : tree->next->suffix_min;
        tree->suffix_min = (tree->root->ckey > tree_next_suff_min->root->ckey)
                               ? tree_next_suff_min
                               : tree;
      }
      tree = tree->prev;
    }
  }

  // TODO(TEAM) unit test
  void InsertTree(const TreePtr& tree1, const TreePtr& tree2) {
    tree1->next = tree2;
    // tree2->prev = tree1;
    if (tree2->prev == nullptr) {
      first_tree = tree1;
    } else {
      tree2->prev->next = tree1;
      tree1->prev = tree2->prev;
    }
    tree2->prev = tree1;
  }

  // TODO(TEAM) unit test
  void RemoveTree(TreePtr tree) {
    auto temp = tree;
    if (tree->prev == nullptr) {
      first_tree = tree->next;
    } else {
      tree->prev->next = tree->next;
    }
    if (temp->next != nullptr) {
      temp->next->prev = temp->prev;
    }
  }

  friend auto operator<<(std::ostream& out, SoftHeap& soft_heap)
      -> std::ostream& {
    out << "SoftHeap: " << soft_heap.rank << "(rank) with trees: \n";
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
  int rank;
  const double epsilon;
  const double r;
};

}  // namespace soft_heap