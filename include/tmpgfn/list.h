#ifndef __TMPGFN_LIST_H__
#define __TMPGFN_LIST_H__

#include <cstddef>
#include <type_traits>

// lists

namespace tmpgfn {
namespace lists {

template <typename... Ts> struct types {};

using nil = types<>;

template <typename List> struct first {};

template <typename First, typename... _> struct first<types<First, _...>> {
  using type = First;
};
template <typename List> using first_t = typename first<List>::type;

template <typename List> struct rest {};

template <typename _, typename... Ts> struct rest<types<_, Ts...>> {
  using type = types<Ts...>;
};

template <> struct rest<nil> { using type = nil; };
template <typename List> using rest_t = typename rest<List>::type;

template <typename List, typename NewFirst> struct insert_front {};

template <typename NewFirst, typename... Ts>
struct insert_front<types<Ts...>, NewFirst> {
  using type = types<NewFirst, Ts...>;
};

template <typename List, typename NewFirst>
using insert_front_t = typename insert_front<List, NewFirst>::type;

template <typename NewLast, typename... Ts> struct insert_back {};

template <typename... Ts, typename NewLast>
struct insert_back<types<Ts...>, NewLast> {
  using type = types<Ts..., NewLast>;
};
template <typename List, typename Last>
using insert_back_t = typename insert_back<List, Last>::type;

template <typename List> struct length_aux {};

template <typename First, typename... Rest>
struct length_aux<types<First, Rest...>> {
  static constexpr size_t value = 1 + length_aux<types<Rest...>>::value;
};

template <> struct length_aux<nil> { static constexpr size_t value = 0; };

template <typename List> struct length {
  static constexpr size_t value = length_aux<List>::value;
};

template <typename List1, typename List2> struct append {};

template <typename List1, typename Head2, typename... Rest2>
struct append<List1, types<Head2, Rest2...>> {
  using type =
      typename append<insert_back_t<List1, Head2>, types<Rest2...>>::type;
};

template <typename List> struct append<List, nil> { using type = List; };

template <typename List1, typename List2>
using append_t = typename append<List1, List2>::type;

template <typename Result, typename List> struct reverse_aux {};

template <typename Result, typename Head, typename... Rest>
struct reverse_aux<Result, types<Head, Rest...>> {
  using ResultWithHeadInserted = insert_front_t<Result, Head>;
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
