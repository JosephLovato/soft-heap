#pragma once
#include <algorithm>
#include <cmath>
#include <compare>
#include <iostream>
#include <iterator>
#include <type_traits>

#include "policies.hpp"
#include "utility.hpp"

namespace soft_heap {

template <policy::TotalOrdered Element, policy::TotalOrderedContainer List,
          int inverse_epsilon>
class FlatNode {
 public:
  FlatNode() = delete;

  constexpr explicit FlatNode(Element&& element) noexcept
      : elements(List{std::forward<Element>(element)}),
        ckey(std::forward<Element>(element)),
        rank(0),
        size(1),
        ckey_present(true) {}

  constexpr explicit FlatNode(int rank, int size, List&& list) noexcept
      : elements(std::move(list)),
        ckey(*std::max_element(elements.begin(), elements.end())),
        rank(rank),
        size(size),
        ckey_present(true) {}

  constexpr explicit FlatNode(const FlatNode& node1,
                              const FlatNode& node2) noexcept
      : rank(std::max(node2.rank, node1.rank) + 1),
        size((rank > ConstCeil(std::log2(inverse_epsilon)) + 5)
                 ? std::max(node2.rank, node1.rank) + 1
                 : 1),
        ckey_present(true) {
    // Sift();
  }

  [[nodiscard]] constexpr auto back() const noexcept -> Element {
    return elements.back();
  }

  constexpr void pop_back() noexcept { elements.pop_back(); }

  constexpr auto operator<=>(const FlatNode& that) const noexcept
      -> std::strong_ordering {
    return this->ckey <=> that.ckey;
  }

  // constexpr auto operator<=>(FlatNode&& that) const noexcept
  //     -> std::strong_ordering {
  //   return this->ckey <=> that.ckey;
  // }

  friend auto operator<<(std::ostream& out, FlatNode node) noexcept
      -> std::ostream& {
    out << "Node: " << node.ckey << " (ckey), rank: " << node.rank
        << ", size: " << node.size;
    out << "\nwith elements: ";
    std::copy(node.elements.begin(), node.elements.end(),
              std::ostream_iterator<Element>(out, ", "));
    return out;
  }

  List elements;
  Element ckey;
  int rank;
  int size;
  bool ckey_present;
};

}  // namespace soft_heap
