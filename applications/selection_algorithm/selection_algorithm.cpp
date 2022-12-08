#include "selection_algorithm.hpp"

using namespace selection_algorithm;

// TODO soft-heap based min-heap selection algorithm
void soft_heap_selection(std::input_iterator auto first,
                         std::input_iterator auto last) noexcept {
  return;
};

// TODO
void standard_heap_selection(std::input_iterator auto first,
                             std::input_iterator auto last) noexcept {
  return;
};

auto selection_algorithm::standard_heap_selection(
    const std::priority_queue<int>& input_heap, const size_t k) noexcept
    -> std::vector<int> {
  return std::vector<int>{};
};

auto selection_algorithm::soft_heap_selection(
    const std::priority_queue<int>& input_heap, const size_t k) noexcept
    -> std::vector<int> {
  return std::vector<int>{};
};