#pragma once
#include <concepts>
#include <queue>

#include "soft_heap.hpp"

namespace selection_algorithm {

auto standard_heap_selection(std::input_iterator auto first,
                             std::input_iterator auto last, size_t k) noexcept
    -> std::vector<int>;

auto standard_heap_selection(const std::vector<int>& input_heap,
                             size_t k) noexcept -> std::vector<int>;

auto standard_heap_selection_vector(std::vector<int>& input, size_t k) noexcept
    -> std::vector<int>;

auto soft_heap_selection(std::input_iterator auto first,
                         std::input_iterator auto last) noexcept
    -> std::vector<int>;

auto soft_heap_selection(const std::vector<int>& input_heap, size_t k) noexcept
    -> std::vector<int>;
;

}  // namespace selection_algorithm