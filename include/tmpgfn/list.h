#ifndef __TMPGFN_LIST_H__
#define __TMPGFN_LIST_H__

#include <cstddef>
#include <type_traits>

// lists

namespace tmpgfn {
namespace lists {

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

template <typename Result, typename List> struct reverse_aux {};

template <typename Result, typename Head, typename... Rest>
struct reverse_aux<Result, types<Head, Rest...>> {
  using ResultWithHeadInserted = typename Result::template insert_front_t<Head>;
  using type =
      typename reverse_aux<ResultWithHeadInserted, types<Rest...>>::type;
};

template <typename Result> struct reverse_aux<Result, nil> {
  using type = Result;
};

template <typename List> struct reverse {
  using type = typename reverse_aux<nil, List>::type;
};

template <typename List> using reverse_t = typename reverse<List>::type;

} // namespace lists
} // namespace tmpgfn

#endif // __TMPGFN_LIST_H__
