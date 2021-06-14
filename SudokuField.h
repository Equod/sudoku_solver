//
// Created by illya on 13/06/21.
//

#ifndef SUDOKU_SOLVER__SUDOKUFIELD_H_
#define SUDOKU_SOLVER__SUDOKUFIELD_H_

#include <array>
#include <bitset>
#include "bit_functions.h"
#include "utility.h"
#include "connected_positions.h"

struct SudokuField {
  SudokuField()
      : field(empty_field()) {
  }
  void InsertNumber(value_type n, size_t pos_x, size_t pos_y) {
    // set value
    field[pos_y * sudoku_size + pos_x] = (1 << n);
    for (const auto&[row, col] : connected_positions[pos_y * sudoku_size + pos_x]) {
      const size_t pos = row * sudoku_size + col;
      if (BIT_COUNT(field[pos]) > 1) {
        CLEAR_VALUE(field[pos], n);
        // remove other if needed
        if (BIT_COUNT(field[pos]) == 1) {
          auto num = get_number_fast(field[pos]);
          InsertNumber(num, col, row);
        }
      }
    }
  }
  void print_as_table() const {
    for (int r = 0; r < sudoku_size; ++r) {
      for (int c = 0; c < sudoku_size; ++c) {
        if (auto n = get_number(field[r * sudoku_size + c]); n > 0) {
          std::cout << n << " ";
        } else {
          std::cout << "  ";
        }
        if (c % sudoku_squares_in_row == sudoku_squares_in_row - 1 && c < sudoku_size - 1) {
          std::cout << "| ";
        }
      }
      if (r % sudoku_squares_in_row == sudoku_squares_in_row - 1 && r < sudoku_size - 1) {
        std::cout << "\n";
        for (int i = 0; i < sudoku_size + sudoku_squares_in_row - 1; ++i) {
          std::cout << "--";
        }
      }
      std::cout << "\n";
    }
  }
  void print_binary() const {
    for (size_t r = 0; r < sudoku_size; ++r) {
      for (size_t c = 0; c < sudoku_size; ++c) {
        size_t pos = r * sudoku_size + c;
        std::cout << "[" << r << " - " << c << "] "
                  << std::bitset<sudoku_size>(field[pos] >> 1) << " " << get_number(field[pos]) << "\n";
      }
      std::cout << "\n";
    }
  }
  template<class ContainerType>
  void ParseRow(const ContainerType& container, size_t row) {
    size_t count = 0;
    for (const auto& item : container) {
      if (item) {
        InsertNumber(item, count, row);
      }
      ++count;
    }
  }

  template<class ContainerType, class Type = value_type>
  void Parse(const ContainerType& param) {
    size_t count = 0;
    for (const auto& row : param) {
      ParseRow(row, count++);
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
    size_t min_val = sudoku_size;
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
    auto pos_x = pos % sudoku_size;
    auto pos_y = pos / sudoku_size;
    for (int i = 1; i <= sudoku_size; ++i) {
      if (*min_it & (1 << i)) {
        auto copy = *this;
        copy.InsertNumber(i, pos_x, pos_y);
        if (copy.solve()) {
          *this = copy;
          return true;
        }
      }
    }
    return false;
  }
  field_type field;
};

#endif //SUDOKU_SOLVER__SUDOKUFIELD_H_
