#ifndef __TMPGFN_MAP_H__
#define __TMPGFN_MAP_H__

#include <tmpgfn/list.h>

#include <cstddef>
#include <type_traits>

namespace tmpgfn {
namespace maps {

using namespace lists;

template <typename List> struct map {};

template <typename Map, typename K> struct get_wrap {};

template <typename First, typename Second, typename K, typename... Rest>
struct get_wrap<map<types<types<First, Second>, Rest...>>, K> {
  using type =
      std::conditional_t<std::is_same_v<K, First>, types<Second>,
                         typename get_wrap<map<types<Rest...>>, K>::type>;
};

template <typename _> struct get_wrap<map<nil>, _> { using type = nil; };

template <typename Map, typename K> class get {
  using get_wrap_type = typename get_wrap<Map, K>::type;

public:
  using type = std::enable_if_t<!std::is_same_v<get_wrap_type, nil>,
                                first_t<get_wrap_type>>;
};
template <typename Map, typename K> using get_t = typename get<Map, K>::type;

template <typename Map, typename K, typename V> struct set {};

template <typename TypesList, typename K, typename V>
struct set<map<TypesList>, K, V> {
  using type = map<insert_front_t<TypesList, types<K, V>>>;
};

template <typename Map, typename K, typename V>
using set_t = typename set<Map, K, V>::type;

} // namespace maps
} // namespace tmpgfn

#endif // __TMPGFN_MAP_H__
