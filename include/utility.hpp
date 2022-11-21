#pragma once

namespace soft_heap {

[[nodiscard]] constexpr auto ConstCeil(double in) noexcept -> int {
  const auto out = static_cast<int>(in);
  return in > out ? out + 1 : out;
}

}  // namespace soft_heap