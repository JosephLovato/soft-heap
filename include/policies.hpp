#pragma once
#include <concepts>

namespace soft_heap::policy {

template <class T>
concept TotalOrdered = requires(T param) { requires std::totally_ordered<T>; };

}  // namespace soft_heap::policy