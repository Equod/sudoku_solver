#include <iostream>
#include <chrono>
#include <vector>
#include "SudokuField.hpp"
#include "connected_positions.hpp"

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
//  auto cp = connected_positions;
//  for (const auto& item : cp[58]) {
//    std::cout << "[ " << item.first << ", " << item.second << " ]\n";
//  }
//  return 0;
  SudokuField field;
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
