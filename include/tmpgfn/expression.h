#ifndef __TMPGFN_CONSTANT_H__
#define __TMPGFN_CONSTANT_H__

#include <type_traits>

namespace tmpgfn {
namespace expressions {

template <typename V, V v> struct expression {
  using type = V;
  static constexpr V value = v;
  static constexpr bool is_expression = true;
};

template <typename V, V v,
          typename std::enable_if_t<std::is_integral<V>::value, bool> = true>
struct constant : public expression<V, v> {
  using type = V;
  static constexpr V value = v;
};
template <typename V> using one = constant<V, static_cast<V>(1)>;
template <typename V> using zero = constant<V, static_cast<V>(0)>;

template <typename E1, typename E2,
          typename std::enable_if_t<E1::is_expression, bool> = true,
          typename std::enable_if_t<E2::is_expression, bool> = true,
          typename std::enable_if_t<
              std::is_same_v<typename E1::type, typename E2::type>, bool> =
              true>
struct validate_expressions {};

template <typename E1, typename E2>
struct add
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value + E2::value)> {
};

template <typename E1,
          typename std::enable_if_t<E1::is_expression, bool> = true>
struct increment : add<E1, one<typename E1::type>> {};

template <typename E1, typename E2>
struct subtract
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value - E2::value)> {
};

template <typename E1,
          typename std::enable_if_t<E1::is_expression, bool> = true>
struct decrement : subtract<E1, one<typename E1::type>> {};

template <typename E1, typename E2>
struct multiply
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value *E2::value)> {
};

template <typename E1, typename E2>
struct divide
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value / E2::value)> {
};

} // namespace expressions
} // namespace tmpgfn

#endif // __TMPGFN_CONSTANT_H__