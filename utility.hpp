//
// Created by illya on 14/06/21.
//

#ifndef SUDOKU_SOLVER__UTILITY_HPP_
#define SUDOKU_SOLVER__UTILITY_HPP_

using value_type = uint16_t;

template<typename ValueType, size_t... Indices>
static constexpr auto generate_array_of_value_internal(ValueType Value, std::integer_sequence<size_t, Indices...>) {
  return std::array{ ((void) Indices, Value)... };
}

template<value_type Value, size_t N>
static constexpr auto generate_array_of_value = []() {
  return generate_array_of_value_internal(Value, std::make_index_sequence<N>{});
};

template<size_t SudokuSize>
static constexpr auto get_empty_field = []() {
  return generate_array_of_value<(1u << (SudokuSize + 1u)) - 2u, SudokuSize * SudokuSize>();
};

template<size_t SudokuSize>
static constexpr auto get_indirect_empty_field = []() {
  return generate_array_of_value_internal(generate_array_of_value<(1u << SudokuSize) - 1u, SudokuSize>(),
                                          std::make_index_sequence<SudokuSize>{});
};

#define CLEAR_VALUE(n, pos) ((n) &= ~(1 << (pos)))

static constexpr auto get_number_fast = [](value_type n) {
  return BIT_POS(n);
};

static constexpr auto get_number = [](value_type n) {
  return BIT_COUNT(n) == 1 ? get_number_fast(n) : 0;
};

template<size_t SquareInRow>
static constexpr auto get_square_index_by_pos = [](size_t pos_x, size_t pos_y) {
  return pos_y / SquareInRow * SquareInRow + pos_x / SquareInRow;
};

#endif //SUDOKU_SOLVER__UTILITY_HPP_
