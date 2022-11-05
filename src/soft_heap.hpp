#pragma once
#include <cmath>
#include <memory>

#include "policies.hpp"
#include "tree.hpp"

namespace soft_heap {

// using policy::TotalOrdered;

template <template <class... T> class List, policy::TotalOrdered Element>
class SoftHeap {
 private:
  using TreePtr = std::shared_ptr<Tree<List, Element>>;
  constexpr auto MakeTreePtr(Element&& element) {
    return std::make_shared<Tree<List, Element>>(
        std::forward<Element>(element));
  }

 public:
  SoftHeap() = delete;
  explicit SoftHeap(Element&& element, double eps = 0.1)
      : first_tree(MakeTreePtr(std::forward<Element>(element))),
        last_tree(first_tree),
        rank(0),
        epsilon(eps),
        r(ceil(log(1 / eps)) + 5) {}
  ~SoftHeap() = default;

  void Meld(SoftHeap P) {
    // TODO(Team)
  }

  // TODO(TEAM) unit test
  void Insert(Element&& e) { Meld(SoftHeap(std::forward<Element>(e))); }

  auto ExtractMin() const -> Element {
    // TODO(TEAM)
    return 0;
  }

  // TODO(TEAM) unit test
  void RemoveTree(TreePtr tree) {
    if (tree->prev == nullptr) {
      first_tree = tree->next;
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
      first_tree = tree1;
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

  // TODO(Team): MergeInto
  // TODO(Team): RepeatedCombine

  TreePtr first_tree;
  TreePtr last_tree;
  int rank;
  const double epsilon;
  const double r;
};

}  // namespace soft_heap