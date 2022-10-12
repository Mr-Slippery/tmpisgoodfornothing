#include "tmpgfn.h"

static void check_lists() {
  // lists

  static_assert(
      // the first of the list of types: [float, char] is: float.
      std::is_same<types<float, char>::first_t, float>::value);
  static_assert(
      // the rest of the list of types: [int, float, char] is: [float, char].
      std::is_same<types<int, float, char>::rest_t, types<float, char>>::value);
  static_assert(
      // the first of the rest of the list of types [int, float, char] is:
      // float.
      std::is_same<types<int, float, char>::rest_t::first_t, float>::value);
  using a_type_list = types<int, double, float>;
  static_assert(
      // by inserting the first element of a list, in front of the
      // rest of the elements of the list, we get the list itself.
      std::is_same<a_type_list::rest_t::insert_front_t<a_type_list::first_t>,
                   a_type_list>::value);
  static_assert(types<int, double, float>::length == 3);
  static_assert(
      // the length of the empty list is zero.
      nil::length == 0);

  using extended_type_list = a_type_list::insert_front_t<char>;
  static_assert(std::is_same<extended_type_list::first_t, char>::value);
  static_assert(extended_type_list::length == 4);

  using empty_list = nil::rest_t;
  static_assert(empty_list::length == 0);
  static_assert(
      // inserting an element in front of the empty list and
      // inserting the same element at the back of the empty list
      // results in the same list.
      std::is_same<empty_list::insert_front_t<int>,
                   empty_list::insert_back_t<int>>::value);

  static_assert(std::is_same<types<int, char>::append_t<types<double, float>>,
                             types<int, char, double, float>>::value);
  static_assert(std::is_same<nil::append_t<types<double, float>>,
                             types<double, float>>::value);
  static_assert(
      std::is_same<types<int, char>::append_t<nil>, types<int, char>>::value);
}

static void check_maps() {
  // maps

  using a_type_map =
      map<types<types<int, char>, types<double, float>, types<char, double>>>;
  static_assert(
      // the value corresponding to key int is: char.
      std::is_same<a_type_map::get_t<int>, char>::value);
  static_assert(std::is_same<a_type_map::get_t<double>, float>::value);
  static_assert(std::is_same<a_type_map::get_t<char>, double>::value);

  // extend a map with a key-value pair, overriding
  // the previous value for key int (char) with double.
  using extended_type_map = a_type_map::set_t<int, double>;
  static_assert(std::is_same<
                extended_type_map,
                map<types<types<int, double>, types<int, char>,
                          types<double, float>, types<char, double>>>>::value);
  static_assert(std::is_same<extended_type_map::get_t<int>, double>::value);

  using empty_map = map<nil>;
  // a map is a list of type pairs.
  static_assert(
      std::is_same<empty_map::set_t<int, char>,
                   map<nil::insert_front_t<types<int, char>>>>::value);
}

int main() {

  check_lists();
  check_maps();

  return 0;
}
