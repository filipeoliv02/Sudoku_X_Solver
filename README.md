# Sudoku X Solver

A high-performance **Sudoku X (Diagonal Sudoku)** solver and generator implemented in C. Developed as a University Project, this application handles grids from  up to  (and theoretically higher) using advanced backtracking and dynamic memory structures.

## 🌟 Overview

Unlike standard Sudoku, **Sudoku X** adds a layer of complexity: the two main diagonals must also contain unique digits from  to . This project explores two distinct implementation philosophies:

* **Fase 1 (Static):** High-speed solving using stack-allocated multidimensional arrays.
* **Fase 2 (Dynamic):** Memory-efficient solving using heap-allocated linked structures, allowing for variable grid sizes without recompilation.

---

## 🚀 Key Features

* **Sudoku X Logic:** Integrated diagonal constraint checking.
* **Variable Dimensions:** Native support for , , , and  puzzles.
* **Algorithm Comparison:** Tools to benchmark different solving strategies.
* **Random Puzzle Generation:** Create valid, solvable Sudoku X boards.
* **Dual File Support:** Read/Write puzzles in both human-readable `.txt` and high-speed binary formats.

---

## 🧠 Tactics & Optimizations

The solver employs several specific heuristics and low-level optimizations to handle the exponential complexity of larger grids:

### 1. Constraint Pruning

The `is_safe` function is optimized to fail fast. It checks constraints in an order designed to catch conflicts early:

1. **Row & Column:** Constant time array lookups.
2. **Subgrid ():** Calculated using .
3. **Conditional Diagonals:** Diagonal checks are only triggered if the cell's coordinates satisfy  or , preventing unnecessary logic execution for the majority of cells.

### 2. Forward Consistency Checking

Before the recursive search begins, a pre-processor validates the initial state of the board. By ensuring the "seed" numbers are consistent with Sudoku X rules, the algorithm avoids exploring massive, impossible branches of the search tree.

### 3. Backtracking Heuristics

The solver uses a depth-first search (DFS) backtracking approach. It minimizes memory overhead by modifying the board "in-place" and reverting changes (backtracking) only when a constraint is violated.

---

## 📂 Project Structure

| Directory | Purpose |
| --- | --- |
| `fase1/` | Array-based implementation (Optimized for speed). |
| `fase2/` | Pointer/Linked implementation (Optimized for flexibility). |
| `data/` | Database of solved and unsolved puzzles. |
| `client_test_files/` | Unit test cases for different grid sizes. |
| `html/` | Doxygen technical documentation (Open `index.html`). |

---

## 🛠️ Setup & Compilation

### Prerequisites

* **CMake** (v3.15 or higher)
* **C Compiler** (GCC, Clang, or MSVC)

### Build Instructions

```bash
# 1. Clone the repo
git clone https://github.com/filipeoliv02/Sudoku_X_Solver.git
cd Sudoku_X_Solver

# 2. Initialize CMake
cmake .

# 3. Build the project
make

```

### Running the Program

```bash
./sudoku

```

This will open the main menu, allowing you to choose between Phase 1 and Phase 2 solvers, generate puzzles, or run performance comparisons.

---

## 📝 Documentation

The codebase is fully documented using Doxygen. To view detailed function signatures, call graphs, and data structures:

1. Navigate to the `html/` directory.
2. Open `index.html` in your preferred web browser.
