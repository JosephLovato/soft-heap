#pragma once
#include <cmath>
#include <memory>

#include "node.hpp"
#include "policies.hpp"
#include "tree.hpp"

namespace soft_heap {

// using policy::TotalOrdered;
template <template <class... T> class List, policy::TotalOrdered Element>
class SoftHeapImpl {
 private:
  using TreePtr = std::shared_ptr<Tree<List, Element>>;
  constexpr auto MakeTreePtr(Element&& element) {
    return std::make_shared<Tree<List, Element>>(
        std::forward<Element>(element));
  }

 public:
  explicit SoftHeapImpl(Element&& element, double&& eps)
      : first_tree(MakeTreePtr(std::forward<Element>(element))),
        last_tree(first_tree),
        rank(0),
        epsilon(eps),
        r(ceil(log(1 / eps)) + 5) {}
  SoftHeapImpl() = delete;
  ~SoftHeapImpl() = default;
  TreePtr first_tree;
  TreePtr last_tree;
  int rank;
  const double epsilon;
  const double r;
};

template <template <class... T> class List, policy::TotalOrdered Element>
class SoftHeap {
 private:
  using TreePtr = std::shared_ptr<Tree<List, Element>>;
  constexpr auto MakeImplPtr(Element&& element, double&& eps) {
    return std::make_unique<SoftHeapImpl<List, Element>>(
        std::forward<Element>(element), std::forward<double>(eps));
  }

 public:
  SoftHeap() = delete;
  explicit SoftHeap(Element&& element, double eps = 0.1)
      : impl(MakeImplPtr(std::forward<Element>(element),
                         std::forward<double>(eps))) {}
  template <class InputIterator>

  // Build from STL style iterators
  SoftHeap(InputIterator first, InputIterator last, double eps = 0.1) {
    impl = MakeImplPtr(std::forward<Element>(*first, eps));
    for (auto it = std::next(first); it != last; ++it) {
      Insert(*it);
    }
  }
  ~SoftHeap() = default;

  // TODO(Team)
  void Meld(SoftHeap&& P) {
    if (P->impl->rank > impl->rank) {
      std::swap(P->impl, impl);  // TODO(team): works on unique?
    }
    MergeInto(P);
    RepeatedCombine(P->impl->rank);
    ~P();
    // call repeatedcombine
  }

  // TODO(Team): unit test
  void MergeInto(SoftHeap&& P) {
    if (P->impl->rank > impl->rank) {
      return;
    }
    auto tree1 = P->impl->first_tree;
    auto tree2 = impl->first_tree;
    while (tree1 != nullptr) {
      while (tree1->rank > tree2->rank) {
        tree2 = tree2->next;
      }
      auto tree1_temp = tree1->next;
      InsertTree(tree1, tree2);
      tree1 = tree1_temp;
    }
  }

  // TODO(Team): unit test
  void RepeatedCombine(int k) {
    auto tree = impl->first_tree;
    while (tree->next != nullptr) {
      if (tree->rank == tree->next->rank) {
        if (tree->next->next == nullptr or
            tree->rank != tree->next->next->rank) {
          tree->root = Node(tree->root, tree->next->root, impl->r);
          tree->rank = tree->root->rank;
          RemoveTree(tree->next);
        }
      } else if (tree->rank > k) {
        break;
      }
      tree = tree->next;
    }
    if (tree->rank > impl->rank) {
      impl->rank = tree->rank;
    }
    UpdateSuffixMin(tree);
  }

  // TODO(TEAM) unit test
  void Insert(Element&& e) const { Meld(SoftHeap(std::forward<Element>(e))); }

  auto ExtractMin() const -> Element {
    // TODO(TEAM)
    return 0;
  }

  // TODO(TEAM) unit test
  void RemoveTree(TreePtr tree) {
    if (tree->prev == nullptr) {
      impl->first_tree = tree->next;
    } else {
      tree->prev->next = tree->next;
    }
    if (tree->next != nullptr) {
      tree->next->prev = tree->prev;
    }
  }

  // TODO(TEAM) unit test
  void InsertTree(TreePtr tree1, TreePtr tree2) {
    tree1->next = tree2;
    if (tree2->prev == nullptr) {
      impl->first_tree = tree1;
    } else {
      tree2->prev->next = tree1;
    }
  }

  // TODO(TEAM) unit test
  void UpdateSuffixMin(TreePtr tree) const {
    while (tree != nullptr) {
      tree->suffix_min = (tree->root->ckey > tree->next->suffix_min->root->ckey)
                             ? tree->next->suffix_min
                             : tree;
      tree = tree->prev;
    }
  }

  std::unique_ptr<SoftHeapImpl<List, Element>> impl;
};

}  // namespace soft_heap