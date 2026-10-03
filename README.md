# CFS-Style Process Scheduler on a 2-3-4 Tree

A simulation of a simplified Linux CFS-style (Completely Fair Scheduler) process scheduler, written in C++. The run queue is a **2-3-4 tree** ordered by each process's current waiting time, in descending order, and the leftmost process (the one with the longest current waiting time) runs next. The tree can be printed both as a 2-3-4 tree and as the equivalent **red-black tree**.

> Coursework for **Algorithms and Data Structures 2** at the School of Electrical Engineering, University of Belgrade (2025/26).

## Features

- **Self-balancing run queue**: inserting into a full node splits it and moves the middle key up to the parent, with splits propagating towards the root, so all leaves stay at the same depth. Deleting an inner key swaps it with its in-order predecessor; an emptied node borrows a key from a sibling or merges with it.
- **Red-black view**: every key carries a colour, so each 2-3-4 node maps to a black key with up to two red children. The tree can be printed in either form.
- **Scheduling simulation**: repeatedly takes the leftmost process and runs it for one time slice (or less, if it needs less to finish), adds the elapsed time to the waiting time of every process, and when a process's waiting time reaches its limit, subtracts the limit and re-inserts the process. A process leaves the tree once it has received its full execution time.
- **Search**: by current waiting and execution time, or by process name.
- **Traversals and output**: level-order (2-3-4), red-black and in-order printing, to the console or to a file. All tree algorithms are iterative (no recursion), as the assignment required.
- **Input**: processes from the keyboard or from a text file.

## Building

C++11 or later, no dependencies:

```sh
g++ -std=c++17 -O2 -o scheduler *.cpp
./scheduler
```

The sources can also be added to a Visual Studio console project.

## Usage

The program is driven by a numbered menu (the menu text is in Serbian):

| Option | Action |
|---|---|
| 1 | Create an empty tree (asks for the time slice) |
| 2 | Find a process by waiting time and execution time |
| 3 | Add processes from standard input (empty line ends input) |
| 4 | Load processes from a file |
| 5 | Look up a process by name |
| 6 | Delete a process |
| 7 | Print the tree (2-3-4 or red-black, to console or file) |
| 8 | Run the scheduling simulation |
| 9 | In-order traversal |
| 10 | Destroy the tree |
| 11 | Exit |

### Input format

One process per line: name, total execution time needed, and maximum waiting time.

```
A 6 10
B 3 4
C 8 12
```

Process names can be up to 256 characters long and must be unique. See [`examples/processes.txt`](examples/processes.txt).

## Project structure

| File | Contents |
|---|---|
| `Key.h/.cpp` | A process: name, required execution time, waiting limit, current waiting/execution time, colour |
| `Node.h/.cpp` | A 2-3-4 tree node with up to three keys and four subtrees; node splitting, borrowing and merging |
| `Tree.h/.cpp` | Insertion, deletion, search, traversals and the scheduling loop |
| `main.cpp` | Menu-driven command-line interface |
