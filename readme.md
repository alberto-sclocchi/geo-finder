# Project 2: GeoFinder

## How to Run

1. Open a new terminal and navigate to the project's root directory (`geo-finder`).

2. Compile the program:

   ```bash
   clang++ -std=c++17 -Wall -o myprogram src/*.cpp
   ```

3. Run the executable:

   ```bash
   ./myprogram
   ```

4. The main menu will display the following options:

   * Prefix Search
   * Exact Lookup
   * Benchmark Testing
   * Quit

5. For both **Prefix Search** and **Exact Lookup**, you will be prompted to choose either the **Trie** or the **Red-Black Tree** implementation.

6. Both data structures return the same search results for identical input. After each operation, the program displays the execution time, allowing you to compare the performance of the two implementations.
