#pragma once
#include <concepts>
// #include <type_traits>

namespace soft_heap::policy {

// concept Arithmetic = std::is_arithmetic_v<T>;
template <class T>
concept TotalOrdered = requires(T param) { requires std::totally_ordered<T>; };

}  // namespace soft_heap::policy