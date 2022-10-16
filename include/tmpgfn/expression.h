#ifndef __TMPGFN_CONSTANT_H__
#define __TMPGFN_CONSTANT_H__

#include <type_traits>

namespace tmpgfn {
namespace expressions {

template <typename V, V v> struct expression {
  using type = V;
  static constexpr V value = v;
};

template <typename V, V v,
          typename std::enable_if_t<std::is_integral<V>::value, bool> = true>
struct constant : public expression<V, v> {};

template <typename V> using one = constant<V, static_cast<V>(1)>;
template <typename V> using zero = constant<V, static_cast<V>(0)>;

template <typename E,
          typename std::enable_if_t<
              std::is_base_of_v<expression<typename E::type, E::value>, E>,
              bool> = true>
struct validate_expression {};

template <
    typename E1, typename E2,
    typename std::enable_if_t<
        std::is_base_of_v<expression<typename E2::type, E2::value>, E2>, bool> =
        true,
    typename std::enable_if_t<
        std::is_same_v<typename E1::type, typename E2::type>, bool> = true>
struct validate_expressions : validate_expression<E1> {};

template <typename E1, typename E2>
struct add
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value + E2::value)> {
};

template <typename E>
struct increment : public add<E, one<typename E::type>> {};

template <typename E1, typename E2>
struct subtract
    : validate_expressions<E1, E2>,
      public expression<typename E1::type,
                        static_cast<typename E1::type>(E1::value - E2::value)> {
};

template <typename E>
struct decrement : public subtract<E, one<typename E::type>> {};

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