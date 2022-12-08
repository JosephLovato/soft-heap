#pragma once
#include <concepts>
#include <queue>

namespace selection_algorithm {

void soft_heap_selection(std::input_iterator auto first,
                         std::input_iterator auto last) noexcept;

void standard_heap_selection(std::input_iterator auto first,
                             std::input_iterator auto last) noexcept;

auto standard_heap_selection(const std::priority_queue<int, std::vector<int>, std::greater<>>& input_heap,
                             const size_t k) noexcept -> std::vector<int>;

auto soft_heap_selection(const std::priority_queue<int, std::vector<int>, std::greater<>>& input_heap,
                         const size_t k) noexcept -> std::vector<int>;

}  // namespace selection_algorithm