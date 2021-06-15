## Sudoku solver project
This project helps to solve sudoku in the fastest way. It takes about 20us = 0.00002s (on intel i7 3rd gen x86_64) to solve the world hardest sudoku (According to the British newspaper The Telegraph)
[Article](https://www.telegraph.co.uk/news/science/science-news/9359579/Worlds-hardest-sudoku-can-you-crack-it.html)
[Description](https://www.conceptispuzzles.com/index.aspx?uri=info/article/424)
### How to use:
1. Create SudokuField structure
```c++
SudokuField<9> field;
```
2. Insert each known numbers using ```insert_number()``` method or call ```parse()``` forwarding the entire schema
```c++
// "0" is invalid number and means no data
static constexpr auto sudoku = []() {
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
field.parse(sudoku);
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
8 6 4 | 2 7 1 | 9 5 3 
9 2 3 | 6 5 4 | 8 7 1 
1 7 5 | 8 9 3 | 2 4 6 
----------------------
2 5 2 | 3 8 7 | 1 9 4 
6 1 9 | 3 4 5 | 7 8 2 
4 8 7 | 1 2 9 | 6 3 5 
----------------------
7 4 1 | 9 3 2 | 5 6 8 
7 4 8 | 5 6 2 | 3 1 9 
5 9 6 | 8 3 1 | 4 2 7 
```
### How it works
```field_type``` is the array of bitset which represents which numbers can be writen in that cell. (Ex: ```0b011100010``` = [1, 5, 6, 7, 8], ```0b0100000000``` = only "8").
insert_number insert the number in the specific cell and clears that number from all other cells in the same row, same column and the same square. ```solve()``` tries to find the cell with minimum contended number. Then tries to insert sequentially each of numbers and tries to solve sudoku (recursively). When the correct combination found, it stops.   

### Performance consideration
To check if a cell is complete (only one bit is 1) the function calls c++20 ```std::popcount``` which is likely translated in assembly instruction avoiding loops. The same approach is used to determine the position of bit one, in this case is called ```std::countr_zero```. 
