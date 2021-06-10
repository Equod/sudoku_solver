## Sudoku solver project
This project helps to solve sudoku in the fastest way. 
### How to use:
1. Create Field structure
```c++
Field field;
```
2. Insert each known numbers using ```InsertNumber()``` method or call ```Parse()``` forwarding the entire schema
```c++
// "0" is invalid number and means no data
static constexpr auto sudoku = []() {
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
field.Parse(sudoku);
```
3. Call ```solve()``` method
```c++
field.solve();
```
4. Print the result as table
```c++
field.print_as_table();
```
