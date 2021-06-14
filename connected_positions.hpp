//
// Created by illya on 14/06/21.
//

#ifndef SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_
#define SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_

#include "utility.hpp"

using position = std::pair<size_t, size_t>; // { row, col }

static constexpr size_t connected_values_count =
    2 * (sudoku_size - 1) + (sudoku_squares_in_row - 1) * (sudoku_squares_in_row - 1);

using connected_pos_type = std::array<position, connected_values_count>;

static constexpr auto has_the_same_square = [](position pos1, position pos2) {
  return get_square_index_by_pos(pos1.second, pos1.first) == get_square_index_by_pos(pos2.second, pos2.first);
};

static constexpr auto get_connected_positions = []() {
  std::array<connected_pos_type, sudoku_size * sudoku_size> result;
  for (size_t ps = 0; ps < sudoku_size * sudoku_size; ++ps) {
    size_t count = 0;
    for (size_t r = 0; r < sudoku_size; ++r) {
      for (size_t c = 0; c < sudoku_size; ++c) {
        position pos{ ps / sudoku_size, ps % sudoku_size };
        if (r == pos.first || c == pos.second || has_the_same_square(pos, { r, c })) {
          if (r != pos.first || c != pos.second) {
            auto idx = count++;
            result[ps][idx].first = r;
            result[ps][idx].second = c;
          }
        }
      }
    }
  }
  return result;
};

static constexpr auto connected_positions = get_connected_positions();

#endif //SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_
