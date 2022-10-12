#ifndef __TMPGFN_H__
#define __TMPGFN_H__

#include <cstddef>
#include <type_traits>

// lists

template <typename... Ts> struct types {};

using nil = types<>;

template <typename First, typename... Ts> struct types<First, Ts...> {
  using this_t = types<First, Ts...>;

  struct first {
    using type = First;
  };
  using first_t = typename first::type;

  struct rest {
    using type = types<Ts...>;
  };
  using rest_t = typename rest::type;

  template <typename NewFirst> struct insert_front {
    using type = types<NewFirst, First, Ts...>;
  };
  template <typename NewFirst>
  using insert_front_t = typename insert_front<NewFirst>::type;

  template <typename Last> struct insert_back {
    using type = types<First, Ts..., Last>;
  };
  template <typename Last>
  using insert_back_t = typename insert_back<Last>::type;

  struct length_aux {
    static constexpr size_t value = 1 + rest_t::length_aux::value;
  };
  static constexpr size_t length = length_aux::value;

  template <typename... OTs> struct append { using type = this_t; };

  template <typename OHead, typename... ORest>
  struct append<types<OHead, ORest...>> {
    using type =
        typename insert_back_t<OHead>::template append_t<types<ORest...>>;
  };

  template <typename... OTs> using append_t = typename append<OTs...>::type;
};

template <> struct types<> {
  struct rest {
    using type = nil;
  };
  using rest_t = typename rest::type;

  template <typename First> struct insert_front { using type = types<First>; };
  template <typename First>
  using insert_front_t = typename insert_front<First>::type;

  template <typename Last> struct insert_back { using type = types<Last>; };
  template <typename Last>
  using insert_back_t = typename insert_back<Last>::type;

  struct length_aux {
    static constexpr size_t value = 0;
  };
  static constexpr size_t length = length_aux::value;

  template <typename... OTs> struct append { using type = nil; };

  template <typename OHead, typename... ORest>
  struct append<types<OHead, ORest...>> {
    using type =
        typename insert_back_t<OHead>::template append_t<types<ORest...>>;
  };

  template <typename... OTs> using append_t = typename append<OTs...>::type;
};

// maps

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
        std::is_same<K, First>::value, types<Second>,
        typename map<types<Rest...>>::template get_wrap<K>::type>;
  };
  template <typename K> class get {
    using get_wrap_type = typename get_wrap<K>::type;

  public:
    using type = std::enable_if_t<!std::is_same<get_wrap_type, nil>::value,
                                  typename get_wrap_type::first_t>;
  };
  template <typename K> using get_t = typename get<K>::type;

  template <typename K, typename V> struct set {
    using type = map<types<types<K, V>, types<First, Second>, Rest...>>;
  };

  template <typename K, typename V> using set_t = typename set<K, V>::type;
};

#endif // __TMPGFN_H__
