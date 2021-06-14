//
// Created by illya on 14/06/21.
//

#ifndef SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_
#define SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_

using position = std::pair<size_t, size_t>; // { row, col }

#define MID ((lo + hi + 1) / 2)

constexpr uint64_t sqrt_helper(uint64_t x, uint64_t lo, uint64_t hi) {
  return lo == hi ? lo : ((x / MID < MID)
      ? sqrt_helper(x, lo, MID - 1) : sqrt_helper(x, MID, hi));
}

constexpr uint64_t ct_sqrt(uint64_t x) {
  return sqrt_helper(x, 0, x / 2 + 1);
}

template<size_t SudokuSize, size_t SqIr = ct_sqrt(SudokuSize)>
static constexpr size_t connected_values_count =
    2 * (SudokuSize - 1) + (SqIr - 1) * (SqIr - 1);

template<size_t SudokuSize>
using connected_pos_type = std::array<position, connected_values_count<SudokuSize>>;

template<size_t SquaresInRow>
static constexpr auto has_the_same_square = [](position pos1, position pos2) {
  return get_square_index_by_pos<SquaresInRow>(pos1.second, pos1.first)
      == get_square_index_by_pos<SquaresInRow>(pos2.second, pos2.first);
};

template<size_t SudokuSize>
static constexpr auto get_connected_positions = []() {
  std::array<connected_pos_type<SudokuSize>, SudokuSize * SudokuSize> result;
  for (size_t ps = 0; ps < SudokuSize * SudokuSize; ++ps) {
    size_t count = 0;
    for (size_t r = 0; r < SudokuSize; ++r) {
      for (size_t c = 0; c < SudokuSize; ++c) {
        position pos{ ps / SudokuSize, ps % SudokuSize };
        if (r == pos.first || c == pos.second || has_the_same_square<ct_sqrt(SudokuSize)>(pos, { r, c })) {
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

#endif //SUDOKU_SOLVER__CONNECTED_POSITIONS_HPP_
