#include <iostream>
#include <bitset>
#include <array>
#include <chrono>
#include "bit_functions.h"

static constexpr size_t sudoku_squares_in_row = 3;
static constexpr size_t sudoku_size = sudoku_squares_in_row * sudoku_squares_in_row;

using value_type = uint16_t;
using field_type = std::array<value_type, sudoku_size * sudoku_size>;

static constexpr auto empty_field = []() {
  field_type ar{};
  for (auto& item : ar) {
    item = (1 << (sudoku_size + 1)) - 2;
  }
  return ar;
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

static constexpr auto get_square_content = [](size_t index) {
  std::array<value_type, sudoku_size> ar{};
  auto idx = 0;
  auto r_start = index / sudoku_squares_in_row * sudoku_squares_in_row;
  auto c_start = (index % sudoku_squares_in_row) * sudoku_squares_in_row;
  for (size_t r = r_start; r < r_start + sudoku_squares_in_row; ++r) {
    for (size_t c = c_start; c < c_start + sudoku_squares_in_row; ++c) {
      ar[idx++] = r * sudoku_size + c;
    }
  }
  return ar;
};

template<size_t... Indices>
static constexpr auto generate_squares_array(std::integer_sequence<size_t, Indices...>) {
  return std::array{ get_square_content(size_t(Indices))... };
}

static constexpr auto square_content = []() {
  return generate_squares_array(std::make_index_sequence<sudoku_size>{});
};

static constexpr auto square_indexes = square_content();

struct Field {
  Field() : field(empty_field()) {
  }
  void InsertNumber(value_type n, size_t pos_x, size_t pos_y) {
    // set value
    field[pos_y * sudoku_size + pos_x] = (1 << n);
    remove_from_row(n, pos_y);
    remove_from_col(n, pos_x);
    remove_from_square(n, get_square_index_by_pos(pos_x, pos_y));
  }
  void remove_from_row(value_type value, size_t row) {
    for (size_t c = 0; c < sudoku_size; ++c) {
      const size_t pos = row * sudoku_size + c;
      if (BIT_COUNT(field[pos]) > 1) {
        CLEAR_VALUE(field[pos], value);
        // remove other if needed
        if (BIT_COUNT(field[pos]) == 1) {
          auto num = get_number_fast(field[pos]);
          InsertNumber(num, c, row);
        }
      }
    }
  }
  void remove_from_col(value_type value, size_t col) {
    for (size_t r = 0; r < sudoku_size; ++r) {
      size_t pos = r * sudoku_size + col;
      if (BIT_COUNT(field[pos]) > 1) {
        CLEAR_VALUE(field[pos], value);
        // remove other if needed
        if (BIT_COUNT(field[pos]) == 1) {
          auto num = get_number_fast(field[pos]);
          InsertNumber(num, col, r);
        }
      }
    }
  }
  void remove_from_square(value_type value, size_t square_index) {
    for (const auto& pos : square_indexes[square_index]) {
      if (BIT_COUNT(field[pos]) > 1) {
        CLEAR_VALUE(field[pos], value);
        // remove other if needed
        if (BIT_COUNT(field[pos]) == 1) {
          auto num = get_number_fast(field[pos]);
          InsertNumber(num, pos % sudoku_size, pos / sudoku_size);
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
  template<class Type = value_type>
  void Parse(const std::array<std::array<Type, sudoku_size>, sudoku_size>& param) {
    for (size_t r = 0; r < sudoku_size; ++r) {
      for (size_t c = 0; c < sudoku_size; ++c) {
        if (param[r][c]) {
          InsertNumber(param[r][c], c, r);
        }
      }
    }
  }
  bool is_solved() const {
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

static constexpr auto hardest_sudoku = []() {
  return std::array{
      std::array<value_type, 9>{ 8 },
      std::array<value_type, 9>{ 0, 0, 3, 6 },
      std::array<value_type, 9>{ 0, 7, 0, 0, 9, 0, 2 },
      std::array<value_type, 9>{ 0, 5, 0, 0, 0, 7 },
      std::array<value_type, 9>{ 0, 0, 0, 0, 4, 5, 7 },
      std::array<value_type, 9>{ 0, 0, 0, 1, 0, 0, 0, 3 },
      std::array<value_type, 9>{ 0, 0, 1, 0, 0, 0, 0, 6, 8 },
      std::array<value_type, 9>{ 0, 0, 8, 5, 0, 0, 0, 1 },
      std::array<value_type, 9>{ 0, 9, 0, 0, 0, 0, 4 }
  };
}();

static constexpr auto sudoku2 = []() {
  return std::array{
      std::array<value_type, 9>{ 0, 0, 5, 3 },
      std::array<value_type, 9>{ 8, 0, 0, 0, 0, 0, 0, 2 },
      std::array<value_type, 9>{ 0, 7, 0, 0, 1, 0, 5 },
      std::array<value_type, 9>{ 4, 0, 0, 0, 0, 5, 3 },
      std::array<value_type, 9>{ 0, 1, 0, 0, 7, 0, 0, 0, 6 },
      std::array<value_type, 9>{ 0, 0, 3, 2, 0, 0, 0, 8 },
      std::array<value_type, 9>{ 0, 6, 0, 5, 0, 0, 0, 0, 9 },
      std::array<value_type, 9>{ 0, 0, 4, 0, 0, 0, 0, 3 },
      std::array<value_type, 9>{ 0, 0, 0, 0, 0, 9, 7 }
  };
}();

int main() {
  Field field;
  auto start_ts = std::chrono::system_clock::now();
  field.Parse(hardest_sudoku);
  if (field.solve()) {
    auto end_ts = std::chrono::system_clock::now();
    field.print_as_table();
    std::cout << "Took: " << std::chrono::duration_cast<std::chrono::microseconds>(end_ts - start_ts).count()
              << " us\n";
  } else {
    std::cout << "Error solving sudoku...\n";
  }
  return 0;
}
