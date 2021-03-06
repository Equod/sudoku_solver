//
// Created by illya on 13/06/21.
//

#ifndef SUDOKU_SOLVER__SUDOKUFIELD_HPP_
#define SUDOKU_SOLVER__SUDOKUFIELD_HPP_

#include <array>
#include <bitset>
#include "bit_functions.hpp"
#include "utility.hpp"
#include "connected_positions.hpp"

template<size_t SudokuSize>
struct SudokuField {
  SudokuField() : field(empty_field<SudokuSize>()) {}
  void insert_number(value_type n, size_t pos_x, size_t pos_y) {
    // set value
    field[pos_y * SudokuSize + pos_x] = (1 << n);
    for (const auto&[row, col] : connected_positions[pos_y * SudokuSize + pos_x]) {
      const size_t pos = row * SudokuSize + col;
      if (BIT_COUNT(field[pos]) > 1) {
        CLEAR_VALUE(field[pos], n);
        // remove other if needed
        if (BIT_COUNT(field[pos]) == 1) {
          auto num = get_number_fast(field[pos]);
          insert_number(num, col, row);
        }
      }
    }
  }
  void print_as_table() const {
    for (int r = 0; r < SudokuSize; ++r) {
      for (int c = 0; c < SudokuSize; ++c) {
        if (auto n = get_number(field[r * SudokuSize + c]); n > 0) {
          std::cout << n << " ";
        } else {
          std::cout << "  ";
        }
        if (c % sudoku_squares_in_row == sudoku_squares_in_row - 1 && c < SudokuSize - 1) {
          std::cout << "| ";
        }
      }
      if (r % sudoku_squares_in_row == sudoku_squares_in_row - 1 && r < SudokuSize - 1) {
        std::cout << "\n";
        for (int i = 0; i < SudokuSize + sudoku_squares_in_row - 1; ++i) {
          std::cout << "--";
        }
      }
      std::cout << "\n";
    }
  }
  void print_binary() const {
    for (size_t r = 0; r < SudokuSize; ++r) {
      for (size_t c = 0; c < SudokuSize; ++c) {
        size_t pos = r * SudokuSize + c;
        std::cout << "[" << r << " - " << c << "] "
                  << std::bitset<SudokuSize>(field[pos] >> 1) << " " << get_number(field[pos]) << "\n";
      }
      std::cout << "\n";
    }
  }
  template<class ContainerType>
  void parse_row(const ContainerType& container, size_t row) {
    size_t count = 0;
    for (const auto& item : container) {
      if (item) {
        insert_number(item, count, row);
      }
      ++count;
    }
  }

  template<class ContainerType>
  void parse(const ContainerType& param) {
    size_t count = 0;
    for (const auto& row : param) {
      parse_row(row, count++);
    }
  }
  [[nodiscard]] bool is_solved() const {
    for (const auto& item : field) {
      if (BIT_COUNT(item) != 1) {
        return false;
      }
    }
    return true;
  }
  bool solve() {
    if (is_solved()) {
      return true;
    }
    auto min_it = field.begin();
    size_t min_val = SudokuSize;
    for (auto it = field.begin(); it != field.end(); ++it) {
      auto cur_val = BIT_COUNT(*it);
      if (cur_val > 1 && cur_val < min_val) {
        min_val = cur_val;
        min_it = it;
      }
    }
    if (min_it == field.end()) {
      return false;
    }
    auto pos = std::distance(field.begin(), min_it);
    auto pos_x = pos % SudokuSize;
    auto pos_y = pos / SudokuSize;
    for (int i = 1; i <= SudokuSize; ++i) {
      if (*min_it & (1 << i)) {
        auto copy = *this;
        copy.insert_number(i, pos_x, pos_y);
        if (copy.solve()) {
          *this = copy;
          return true;
        }
      }
    }
    return false;
  }
  static constexpr auto connected_positions = get_connected_positions<SudokuSize>();
  static constexpr size_t sudoku_squares_in_row = ct_sqrt(SudokuSize);
  using field_type = std::array<value_type, SudokuSize * SudokuSize>;
  field_type field;
};

#endif //SUDOKU_SOLVER__SUDOKUFIELD_HPP_
