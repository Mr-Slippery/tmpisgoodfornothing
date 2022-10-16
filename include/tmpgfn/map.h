#ifndef __TMPGFN_MAP_H__
#define __TMPGFN_MAP_H__

#include <cstddef>
#include <type_traits>

#include <tmpgfn/list.h>

namespace tmpgfn {
namespace maps {

using namespace lists;

template <typename... Ts> struct map {};

template <> struct map<nil> {
  template <typename K> struct get_wrap { using type = nil; };

  template <typename K, typename V> struct set {
    using type = map<types<types<K, V>>>;
  };
  template <typename K, typename V> using set_t = typename set<K, V>::type;
};

template <typename First, typename Second, typename... Rest>
struct map<types<types<First, Second>, Rest...>> {
  template <typename K> struct get_wrap {
    using type = std::conditional_t<
        std::is_same_v<K, First>, types<Second>,
        typename map<types<Rest...>>::template get_wrap<K>::type>;
  };
  template <typename K> class get {
    using get_wrap_type = typename get_wrap<K>::type;

  public:
    using type = std::enable_if_t<!std::is_same_v<get_wrap_type, nil>,
                                  typename get_wrap_type::first_t>;
  };
  template <typename K> using get_t = typename get<K>::type;

  template <typename K, typename V> struct set {
    using type = map<types<types<K, V>, types<First, Second>, Rest...>>;
  };

  template <typename K, typename V> using set_t = typename set<K, V>::type;
};

} // namespace maps
} // namespace tmpgfn

#endif // __TMPGFN_MAP_H__
