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
It prints:
```text
6 4 5 | 3 2 7 | 1 9 8 
8 3 1 | 9 5 4 | 6 2 7 
9 7 2 | 6 1 8 | 5 4 3 
----------------------
4 8 6 | 1 9 5 | 3 7 2 
2 1 9 | 8 7 3 | 4 5 6 
7 5 3 | 2 4 6 | 9 8 1 
----------------------
3 6 7 | 5 8 2 | 1 1 9 
5 9 4 | 7 6 1 | 2 3 8 
1 2 8 | 4 3 9 | 7 6 5 
```
### How it works
```field_type``` is the array of bitset which represents which numbers can be writen in that cell. (Ex: ```0b011100010``` = [1, 5, 6, 7, 8], ```0b0100000000``` = only "8").
InsertNumber insert the number in the specific cell and clears that number from all other cells in the same row, same column and the same square. ```solve()``` tries to find the cell with minimum contended number. Then tries to insert sequentially each of numbers and tries to solve sudoku (recursively). When the correct combination found, it stops.   

### Performance consideration
To check if a cell is complete (only one bit is 1) the function calls c++20 ```std::popcount``` which is likely translated ins assembly instruction avoiding loops. The same approach is used to determine the position of bit one, in this case is called ```std::countr_zero```. 
