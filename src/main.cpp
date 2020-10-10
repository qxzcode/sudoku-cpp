#include <iostream>
using std::cout;
using std::endl;

#include <random>
#include <cstdint>
#include <limits>
#include <bitset>


/// The generator used for randomness.
std::default_random_engine random_engine{std::random_device()()};

/// Type used to store an individual digit, 0-8.
using digit_t = std::uint_fast8_t;

/// A placeholder/sentinel digit_t value.
constexpr digit_t INVALID_DIGIT = std::numeric_limits<digit_t>::max();

/// Represents a set of sudoku digits.
class digit_set {
private:
    using bitset_t = std::bitset<9>;
    bitset_t bitset;
    
    digit_set(bitset_t bits) noexcept : bitset(std::move(bits)) {}
    
public:
    /// Default constructor: include all 9 digits.
    constexpr digit_set() noexcept : bitset(0xffffffff) {}
    
    /// Remove a random digit from this set and return it.
    digit_t pick_digit() {
        size_t count = bitset.count();
        if (count == 0) return INVALID_DIGIT;
        
        // get a random number from [0, <# of digits in the set>)
        size_t i = random_engine() % count;
        
        // return the i-th set bit
        for (digit_t d = 0; d < 9; d++) {
            if (test(d)) {
                if (i == 0) {
                    remove(d);
                    return d;
                } else {
                    i--;
                }
            }
        }
        
        // this should never be reached
        return INVALID_DIGIT;
    }
    
    /// Tests if the given digit is in this set.
    bool test(digit_t digit) const {
        return bitset[digit];
    }
    
    /// Removes the given digit from this set.
    void remove(digit_t digit) {
        bitset[digit] = false;
    }
    
    /// Returns the intersection of this set with another.
    digit_set operator&(const digit_set& rhs) const noexcept {
        return digit_set(bitset & rhs.bitset);
    }
    
    /// Returns the number of digits in this set.
    size_t count() const noexcept {
        return bitset.count();
    }
};

/// Represents a 9x9 grid of digits.
using sudoku_grid = digit_t[9][9];

/// Stores the backtracking state.
/// Each digit_set here stores which digits are *unfilled* in each group.
struct group_state {
    digit_set rows[9];
    digit_set cols[9];
    digit_set squares[3][3];
    
    size_t row = 0;
    size_t col = 0;
    
    digit_set& current_row() { return rows[row]; }
    const digit_set& current_row() const { return rows[row]; }
    
    digit_set& current_col() { return cols[col]; }
    const digit_set& current_col() const { return cols[col]; }
    
    digit_set& current_square() { return squares[row / 3][col / 3]; }
    const digit_set& current_square() const { return squares[row / 3][col / 3]; }
    
    bool is_finished() const {
        return row >= 9;
    }
    
    /// Returns the next state that results from filling the current position
    /// with the given digit.
    group_state get_next_state(digit_t digit) const {
        // start with a copy of the current state
        group_state next_state = *this;
        
        // fill in the current position with the given digit
        next_state.current_row().remove(digit);
        next_state.current_col().remove(digit);
        next_state.current_square().remove(digit);
        
        // move to the next grid position
        if (next_state.col == 8) {
            next_state.row++;
            next_state.col = 0;
        } else {
            next_state.col++;
        }
        
        return next_state;
    }
};

/// Finds a valid sudoku grid using a recursive backtracking algorithm.
bool find_solution(sudoku_grid& grid, const group_state state = {}) {
    if (state.is_finished()) {
        // this state is a complete and valid solution
        return true;
    }
    
    // get the set of valid digits (the intersection of the valid digits for
    // the current row, column, and 3x3 square)
    digit_set next_digit_set = state.current_row() &
                               state.current_col() &
                               state.current_square();
    
    // try valid digits in a random order until the first one that results in a
    // valid solution
    digit_t next_digit;
    while ((next_digit = next_digit_set.pick_digit()) != INVALID_DIGIT) {
        if (find_solution(grid, state.get_next_state(next_digit))) {
            // a solution was found; fill in the square from this step in the
            // final answer grid
            grid[state.row][state.col] = next_digit;
            return true;
        }
    }
    
    // couldn't find a valid solution from this state
    return false;
}


/// Prints a sudoku grid as a 9x9 block of digits.
void print_grid(const sudoku_grid& grid) {
    for (const auto& row : grid) {
        for (const auto& digit : row) {
            cout << int(digit) + 1;
        }
        cout << '\n';
    }
    cout << std::flush;
}


/// The entry point to the program.
int main() {
    sudoku_grid grid = {};
    if (find_solution(grid)) {
        print_grid(grid);
        return 0;
    } else {
        cout << "No solution found. This is most likely a bug." << endl;
        return 1;
    }
}
