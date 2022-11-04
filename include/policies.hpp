#pragma once
#include <type_traits>

namespace soft_heap::policy {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

}