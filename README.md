# Custom STL and Smart Pointer Library

This is a learning project where I rebuild some of C++'s most important standard
library pieces from scratch — the containers and smart pointers you normally get
for free from `std::`. The goal isn't to replace the real thing; it's to actually
understand how they work under the hood by implementing them myself.

## How this project was built (honesty note)

I used an AI assistant to set up the **scaffolding** and **testing infrastructure** —
the class layouts, method signatures, build configuration, and the test harness.
That part is the boilerplate I wanted out of the way.

**The actual implementations are mine.** Every algorithm — the memory management,
the pointer surgery, the reference counting, the tree logic — I write and reason
through myself. The scaffold files contain `// TODO` comments describing what each
method should do; the working code that fills them in is my own work. This README
marks which pieces are finished so you can tell what's me vs. what's still a stub.

## What's being built and why

Each piece is chosen because it teaches something the others don't, not just to
pad out a list. Here's the plan in plain terms:

| Piece | What it is (in plain English) | Why it's worth building |
|-------|-------------------------------|-------------------------|
| **Vector** ✅ | A list that grows automatically as you add things, stored in one continuous block of memory. | Teaches raw memory management — allocating, growing, and cleaning up by hand. The foundation everything else builds on. |
| **List** | A chain of items where each one points to its neighbors, instead of sitting in one block. | Teaches a totally different memory model (lots of small linked pieces) and how to write a real *iterator* that walks the chain. |
| **UniquePtr** | A pointer that owns one object and automatically deletes it when it goes away. Only one owner allowed. | Teaches ownership — the idea that frees you from manually deleting memory and causing leaks. |
| **SharedPtr / WeakPtr** | Pointers that let several owners share one object, which is deleted only when the last owner is done. WeakPtr watches without owning. | Teaches *reference counting* — keeping a tally of owners so the object lives exactly as long as it's needed. |
| **Map** | A dictionary: look things up by a key (like a name) to get a value (like a phone number), kept in sorted order. | Teaches tree data structures and searching — the logic behind fast lookups. |
| **Stack** | A pile where you only add and remove from the top (last in, first out). | Teaches how a simple, restricted interface can be built on top of a container you already have (it reuses Vector). |
| **Queue** | A line where you add at the back and remove from the front (first in, first out). | Same idea as Stack, but shows why the *choice* of underlying container matters (it reuses List for speed). |

✅ = implemented and tested. Everything else is currently scaffolded with stubs and
is actively being implemented.

## Quick example (Vector)

```cpp
#include "vector.h"

int main() {
    Vector<int> numbers(5, 42);   // 5 elements, all 42

    numbers.push_back(100);
    numbers.push_back(200);

    std::cout << "Size: "  << numbers.size()  << std::endl;
    std::cout << "First: " << numbers.front() << std::endl;
    std::cout << "Last: "  << numbers.back()  << std::endl;

    for (int num : numbers) {
        std::cout << num << " ";
    }
    return 0;
}
```

## Project layout

```
├── include/
│   ├── vector.h / vector.hpp          # Growable array        (done)
│   ├── list.h / list.hpp              # Doubly-linked list    (scaffold)
│   ├── unique_ptr.h / unique_ptr.hpp  # Exclusive-owner ptr   (scaffold)
│   ├── shared_ptr.h / shared_ptr.hpp  # Shared + weak ptrs    (scaffold)
│   ├── map.h / map.hpp                # Key/value tree        (scaffold)
│   ├── stack.h / stack.hpp            # LIFO adapter          (scaffold)
│   └── queue.h / queue.hpp            # FIFO adapter          (scaffold)
├── tests/
│   └── test_vector_gtest.cpp          # 29 tests for Vector
├── benchmarks/
│   └── benchmark_vector.cpp           # Vector vs std::vector timing
├── docs/                              # Performance charts + scripts
├── data/                              # Benchmark results (JSON)
├── CMakeLists.txt                     # Build configuration
└── requirements.txt                   # Python deps for charts
```

Each `.h` file holds the class design and documented method signatures; each
`.hpp` holds the implementations (or, for unfinished pieces, the `// TODO` stubs).

## How it works internally

Storage is managed by hand instead of leaning on `std::` helpers. For example,
Vector uses `malloc`/`free` for raw memory and placement `new` to build objects in
it, doubling its capacity when it runs out of room — just like the real
`std::vector`. The smart pointers manage their objects' lifetimes with destructors
and (for SharedPtr) a small shared counter. Getting the construct/destroy and
allocate/free steps in the right order is the trickiest and most educational part.

## Building and testing

### First-time setup (Google Test)

```bash
./scripts/setup_gtest.sh
```

### Build and run the tests

```bash
mkdir build && cd build
cmake ..
make
./test_vector_gtest
```

On macOS, if `cmake` isn't on your PATH:

```bash
/Applications/CMake.app/Contents/bin/cmake ..
```

## Benchmarking and charts

A benchmark compares this Vector against `std::vector` on `push_back` and writes
the results to JSON, which a Python script turns into an interactive chart.

```bash
# 1. Run the benchmark (creates data/benchmark_data.json)
cd build && ./benchmark_vector

# 2. Generate the chart
cd ../docs/scripts && python3 generate_plotly_charts.py

# 3. Open it
open ../charts/performance_chart.html
```

Install the Python dependencies first with `pip install -r requirements.txt`.

## Requirements

- C++17
- CMake 3.16 or newer
- Python 3 (only for the performance charts)
- Google Test (the setup script handles it)
