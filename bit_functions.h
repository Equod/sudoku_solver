//
// Created by illya on 13/06/21.
//

#ifndef SUDOKU_SOLVER__BIT_FUNCTIONS_H_
#define SUDOKU_SOLVER__BIT_FUNCTIONS_H_

#if defined(__has_include)
#if __has_include(<bit>)
#include <bit>
#define BIT_COUNT(n) std::popcount(n)
#define BIT_POS(n) std::countr_zero(n)
#endif
#endif

#if !defined(BIT_COUNT) && !defined(BIT_POS)
#if defined(__GNUC__) && (__GNUC__ > 5)
#define HAS_BUILTIN_BITS
#elif defined(__has_builtin)
#if __has_builtin(__builtin_popcount) && __has_builtin(__builtin_ctz)
#define HAS_BUILTIN_BITS
#endif // __has_builtin
#endif // __GNUC__
#endif // defined(POPCOUNT)


#if !defined(BIT_COUNT) && !defined(BIT_POS) && defined(HAS_BUILTIN_BITS)
#define BIT_COUNT(x) __builtin_popcount(x)
#define BIT_POS(n) __builtin_ctz (n)
#endif



#endif //SUDOKU_SOLVER__BIT_FUNCTIONS_H_
