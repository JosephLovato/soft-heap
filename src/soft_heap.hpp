#pragma once
#include <cmath>
#include <memory>

#include "policies.hpp"
#include "tree.hpp"

namespace soft_heap {

// using policy::TotalOrdered;

class SoftHeapError {
 public:
  SoftHeapError() : r(epsilon + 1) {}
  const double epsilon = 3;
  const double r;
};

template <template <class... T> class List, policy::TotalOrdered Element>
class SoftHeap : public SoftHeapError {
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

  TreePtr first_tree;
  TreePtr last_tree;
  int rank;
  const double epsilon;
  const double r;
};

}  // namespace soft_heap