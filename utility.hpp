//
// Created by illya on 14/06/21.
//

#ifndef SUDOKU_SOLVER__UTILITY_HPP_
#define SUDOKU_SOLVER__UTILITY_HPP_

static constexpr size_t sudoku_squares_in_row = 3;
static constexpr size_t sudoku_size = sudoku_squares_in_row * sudoku_squares_in_row;

using value_type = uint16_t;
using field_type = std::array<value_type, sudoku_size * sudoku_size>;
using additional_field_type = std::array<value_type, sudoku_size>;

template<typename ValueType, size_t... Indices>
static constexpr auto generate_array_of_value_internal(ValueType Value, std::integer_sequence<size_t, Indices...>) {
  return std::array{ ((void) Indices, Value)... };
}

template<value_type Value, size_t N>
static constexpr auto generate_array_of_value = []() {
  return generate_array_of_value_internal(Value, std::make_index_sequence<N>{});
};

static constexpr auto empty_field = []() {
  field_type ar{};
  for (auto& item : ar) {
    item = (1 << (sudoku_size + 1)) - 2;
  }
  return ar;
};

static constexpr auto additional_empty_value = []() {
  return (1 << (sudoku_size)) - 1;
};

#define CLEAR_VALUE(n, pos) ((n) &= ~(1 << (pos)))

static constexpr auto get_number_fast = [](value_type n) {
  return BIT_POS(n);
};

static constexpr auto get_number = [](value_type n) {
  return BIT_COUNT(n) == 1 ? get_number_fast(n) : 0;
};

static constexpr auto get_square_index_by_pos = [](size_t pos_x, size_t pos_y) {
  return pos_y / sudoku_squares_in_row * sudoku_squares_in_row + pos_x / sudoku_squares_in_row;
};

#endif //SUDOKU_SOLVER__UTILITY_HPP_
