#include "tmpgfn/tmpgfn.h"

#include <cstdint>

static void check_lists() {
  using namespace tmpgfn::lists;

  // the first of the list of types: [float, char] is: float.
  static_assert(std::is_same_v<first_t<types<float, char>>, float>);
  // the rest of the list of types: [int, float, char] is: [float, char].
  static_assert(
      std::is_same_v<rest_t<types<int, float, char>>, types<float, char>>);
  // the first of the rest of the list of types [int, float, char] is:
  // float.
  static_assert(
      std::is_same_v<first_t<rest_t<types<int, float, char>>>, float>);
  using a_type_list = types<int, double, float>;
  // by inserting the first element of a list, in front of the
  // rest of the elements of the list, we get the list itself.
  static_assert(
      std::is_same_v<insert_front_t<rest_t<a_type_list>, first_t<a_type_list>>,
                     a_type_list>);
  static_assert(length<types<int, double, float>>::value == 3);
  // the length of the empty list is zero.
  static_assert(length<nil>::value == 0);

  using extended_type_list = insert_front_t<a_type_list, char>;
  static_assert(std::is_same_v<first_t<extended_type_list>, char>);
  static_assert(length<extended_type_list>::value == 4);

  using empty_list = rest_t<nil>;
  static_assert(length<empty_list>::value == 0);
  // inserting an element in front of the empty list and
  // inserting the same element at the back of the empty list
  // results in the same list.
  static_assert(std::is_same_v<insert_front_t<empty_list, int>,
                               insert_back_t<empty_list, int>>);
  static_assert(std::is_same_v<insert_back_t<insert_back_t<nil, int>, double>,
                               types<int, double>>);

  static_assert(std::is_same_v<append_t<types<int, char>, types<double, float>>,
                               types<int, char, double, float>>);
  static_assert(std::is_same_v<append_t<nil, types<double, float>>,
                               types<double, float>>);
  static_assert(
      std::is_same_v<append_t<types<int, char>, nil>, types<int, char>>);
  static_assert(std::is_same_v<reverse_t<nil>, nil>);
  static_assert(std::is_same_v<reverse_t<types<int, char, double>>,
                               types<double, char, int>>);
}

static void check_maps() {
  using namespace tmpgfn::lists;
  using namespace tmpgfn::maps;

  using a_type_map =
      map<types<types<int, char>, types<double, float>, types<char, double>>>;
  // the value corresponding to key int is: char.
  static_assert(std::is_same_v<get_t<a_type_map, int>, char>);
  static_assert(std::is_same_v<get_t<a_type_map, double>, float>);
  static_assert(std::is_same_v<get_t<a_type_map, char>, double>);

  // extend a map with a key-value pair, overriding
  // the previous value for key int (char) with double.
  using extended_type_map = set_t<a_type_map, int, double>;
  static_assert(
      std::is_same_v<extended_type_map,
                     map<types<types<int, double>, types<int, char>,
                               types<double, float>, types<char, double>>>>);
  static_assert(std::is_same_v<get_t<extended_type_map, int>, double>);

  using empty_map = map<nil>;
  // a map is a list of type pairs.
  static_assert(std::is_same_v<set_t<empty_map, int, char>,
                               map<insert_front_t<nil, types<int, char>>>>);
}

void check_constants() {
  using namespace tmpgfn::expressions;

  using i_1 = one<int>;
  static_assert(std::is_same_v<i_1::type, int>);
  static_assert(i_1::value == 1);
  using i_2 = add<i_1, i_1>;
  static_assert(std::is_same_v<i_2::type, int>);
  static_assert(i_2::value == 2);
  using i_3 = add<i_1, i_2>;
  static_assert(std::is_same_v<i_3::type, int>);
  static_assert(i_3::value == 3);
  using i_4 = increment<i_3>;
  static_assert(std::is_same_v<i_4::type, int>);
  static_assert(i_4::value == 4);
  using i_6 = constant<int, 6>;
  static_assert(std::is_same_v<i_6::type, int>);
  static_assert(i_6::value == 6);
  using i_5 = decrement<i_6>;
  static_assert(std::is_same_v<i_5::type, int>);
  static_assert(i_5::value == 5);
  using i_8 = multiply<i_2, i_4>;
  static_assert(std::is_same_v<i_8::type, int>);
  static_assert(i_8::value == 8);
  using i_7 = divide<constant<int, 15>, i_2>;
  static_assert(std::is_same_v<i_7::type, int>);
  static_assert(i_7::value == 7);

  using u8_255 = constant<std::uint8_t, 255>;
  using u8_0 = increment<u8_255>;
  static_assert(u8_0::value == 0u);
  static_assert(std::is_same_v<u8_0::type, std::uint8_t>);
}

int main() {
  check_lists();
  check_maps();
  check_constants();

  return 0;
}
