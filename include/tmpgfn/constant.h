#ifndef __TMPGFN_CONSTANT_H__
#define __TMPGFN_CONSTANT_H__

#include <type_traits>

namespace tmpgfn {
namespace constants {

template <typename V, V v,
          typename std::enable_if_t<std::is_integral<V>::value, bool> = true>
struct constant {
  using type = V;
  static constexpr V value = v;
  using inc = constant<V, static_cast<V>(value + static_cast<V>(1))>;
  using dec = constant<V, static_cast<V>(value - static_cast<V>(1))>;
};

} // namespace constants
} // namespace tmpgfn

#endif // __TMPGFN_CONSTANT_H__