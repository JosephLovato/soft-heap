#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <set>
#include <type_traits>

#include "flat_tree.hpp"
#include "policies.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element,
          policy::TotalOrderedContainer List = std::vector<Element>,
          int inverse_epsilon = 8>
class FlatSoftHeap {
 public:
  using TreeList = std::list<FlatTree<Element, List, inverse_epsilon>>;
  using TreeListIt = typename TreeList::iterator;

  constexpr explicit FlatSoftHeap(Element&& element) noexcept
      : epsilon(1.0 / inverse_epsilon), c_size(1) {
    trees.emplace_back(std::forward<Element>(element));
    trees.begin()->min_ckey = trees.begin();
  }

  constexpr FlatSoftHeap(std::input_iterator auto first,
                         std::input_iterator auto last) noexcept
      : FlatSoftHeap(std::move(*first)) {
    std::for_each(std::next(first), last,
                  [&](auto&& e) { Insert(std::forward<Element>(e)); });
  }

  constexpr void Insert(Element e) noexcept {
    ++c_size;
    auto first_tree = trees.begin();
    if (std::ssize(trees) != 0 and first_tree->rank() == 0) {
      auto& node_heap = first_tree->node_heap;
      node_heap.emplace_back(std::move(e));
      auto& new_root =
          node_heap[0] > node_heap[1] ? node_heap[1] : node_heap[0];
      new_root.size = 1;
      ++new_root.rank;
      for (auto tree = trees.begin(); tree != trees.end();
           std::advance(tree, 1)) {
        auto&& next_tree = std::next(tree);
        if (next_tree != trees.end() and tree->rank() == next_tree->rank()) {
          auto& node_heap = tree->node_heap;
          auto& next_heap = next_tree->node_heap;
          node_heap.insert(node_heap.end(),
                           std::make_move_iterator(next_heap.begin()),
                           std::make_move_iterator(next_heap.end()));
          auto& new_root =
              node_heap[0] > next_heap[0] ? next_heap[0] : node_heap[0];
          new_root.size =
              (tree->rank() > ConstCeil(std::log2(inverse_epsilon)) + 5)
                  ? new_root.size + 1
                  : 1;
          ++new_root.rank;
          tree->SiftInsert();
          trees.erase(next_tree);
          std::advance(tree, -1);
        } else if (tree->rank() > 0) {
          UpdateSuffixMin(tree);
          return;
        }
      }
    } else {
      trees.emplace_front(std::forward<Element>(e));
      trees.begin()->min_ckey = trees.begin();
    }
  }

  constexpr void Meld(FlatSoftHeap&& P) noexcept {
    if (P.rank() > rank()) {
      trees.swap(P.trees);
    }
    const auto p_rank = P.rank();
    trees.merge(P.trees);

    for (auto tree = trees.begin(); tree != trees.end();
         std::advance(tree, 1)) {
      auto&& next_tree = std::next(tree);
      if (next_tree != trees.end() and tree->rank() == next_tree->rank()) {
        auto& node_heap = tree->node_heap;
        auto& next_heap = next_tree->node_heap;
        node_heap.insert(node_heap.end(),
                         std::make_move_iterator(next_heap.begin()),
                         std::make_move_iterator(next_heap.end()));
        auto& new_root =
            node_heap[0] > next_heap[0] ? next_heap[0] : node_heap[0];
        new_root.size =
            (tree->rank() > ConstCeil(std::log2(inverse_epsilon)) + 5)
                ? new_root.size + 1
                : 1;
        ++new_root.rank;
        tree->Sift();
        trees.erase(next_tree);
        std::advance(tree, -1);
      } else if (tree->rank() > p_rank) {
        UpdateSuffixMin(tree);
        return;
      }
    }
    UpdateSuffixMin(std::prev(trees.end()));
  }

  [[nodiscard]] constexpr auto ExtractMin() noexcept {
    const auto& min_tree = trees.front().min_ckey;
    auto& x = min_tree->node_heap[0];
    const auto first_elem = x.back();
    x.pop_back();
    if (2 * std::ssize(x.elements) < x.size) {
      if (std::ssize(min_tree->node_heap) > 1) {  // Check if leaf
        auto& min_node_heap = min_tree->node_heap;
        const auto min_child_idx = std::ssize(min_node_heap) > 2 and
                                           min_node_heap[1] > min_node_heap[2]
                                       ? 2
                                       : 1;
        auto& min_elements = min_node_heap[min_child_idx].elements;
        min_elements.insert(min_elements.end(),
                            std::make_move_iterator(x.elements.begin()),
                            std::make_move_iterator(x.elements.end()));
        std::pop_heap(min_node_heap.begin(), min_node_heap.end(),
                      std::greater<>());
        min_node_heap.pop_back();
        UpdateSuffixMin(min_tree);
      } else if (x.elements.empty()) {
        if (min_tree != trees.begin()) {
          const auto prev = std::prev(min_tree);
          trees.erase(min_tree);
          UpdateSuffixMin(prev);
        } else {
          trees.erase(min_tree);
        }
      }
    }
    --c_size;
    return first_elem;
  }

  [[nodiscard]] auto ExtractMinC() noexcept
      -> std::pair<Element, std::vector<Element>> {
    const auto& min_tree = trees.front().min_ckey;
    auto& x = min_tree->node_heap[0];
    Element first_elem = x.back();
    std::vector<Element> corrupted_elements;
    x.pop_back();
    if (first_elem == x.ckey) {
      x.ckey_present = false;
      corrupted_elements.push_back(first_elem);
    }
    if (2 * std::ssize(x.elements) < x.size) {
      if (std::ssize(min_tree->node_heap) > 1) {  // Check if leaf
        auto& min_node_heap = min_tree->node_heap;
        const auto min_child_idx = std::ssize(min_node_heap) > 2 and
                                           min_node_heap[1] > min_node_heap[2]
                                       ? 2
                                       : 1;
        auto& min_elements = min_node_heap[min_child_idx].elements;
        min_elements.insert(min_elements.end(),
                            std::make_move_iterator(x.elements.begin()),
                            std::make_move_iterator(x.elements.end()));
        if (x.ckey_present) {
          corrupted_elements.push_back(x.ckey);
        }
        std::pop_heap(min_node_heap.begin(), min_node_heap.end(),
                      std::greater<>());
        min_node_heap.pop_back();
        UpdateSuffixMin(min_tree);
      } else if (x.elements.empty()) {
        if (min_tree != trees.begin()) {
          const auto prev = std::prev(min_tree);
          trees.erase(min_tree);
          UpdateSuffixMin(prev);
        } else {
          trees.erase(min_tree);
        }
      }
    }
    --c_size;
    return std::make_pair(first_elem, corrupted_elements);
  }

  friend auto operator<<(std::ostream& out, FlatSoftHeap& soft_heap) noexcept
      -> std::ostream& {
    out << "SoftHeap: " << soft_heap.rank() << "(rank) with trees: \n";
    for (auto&& x : soft_heap.trees) {
      out << "-------------------\n" << x << '\n';
    }
    out << std::endl;
    return out;
  }

  TreeList trees;

  [[nodiscard]] constexpr auto rank() const noexcept {
    return trees.back().rank();
  }

  constexpr void UpdateSuffixMin(TreeListIt it) noexcept {
    std::advance(it, 1);
    while (it != trees.begin()) {
      std::advance(it, -1);
      it->min_ckey =
          (std::next(it) == trees.end() or
           std::next(it)->min_ckey->node_heap[0].ckey >= it->node_heap[0].ckey)
              ? it
              : std::next(it)->min_ckey;
    }
  }

  [[nodiscard]] auto size() const noexcept { return c_size; }

  const double epsilon;

 private:
  size_t c_size{};
};

}  // namespace soft_heap
