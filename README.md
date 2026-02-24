# 🧠 Fundamental Algorithms & Data Structures in C
A professional collection of fundamental algorithms and data structures implemented in C, focusing on memory management, pointer manipulation, and time complexity analysis.

![C](https://img.shields.io/badge/Language-C-blue.svg)
![CS](https://img.shields.io/badge/Focus-Algorithm%20Analysis-orange.svg)
![Status](https://img.shields.io/badge/Academic-Projects-success.svg)

## 📖 Project Overview
This repository contains a comprehensive collection of fundamental algorithms and data structures implemented in C. Beyond simple implementation, the project focuses on **empirical performance analysis**, comparing different approaches through automated testing and complexity measurement.

## 📊 Performance Analysis & Profiling
A key feature of this repository is the use of a custom **Profiler** tool (`Profiler.h`). 
- **Metrics**: Each algorithm is instrumented to count **Assignments** and **Comparisons**.
- **Cases**: Analysis is performed for **Best**, **Average**, and **Worst** scenarios.
- **Visualization**: The generated data can be used to create charts that experimentally validate theoretical time complexities like $O(n^2)$ or $O(n \log n)$.

---

## 🚀 Repository Contents

### 1. Sorting & Selection
* **Direct Sorting Methods**: Implementation and comparison of Bubble Sort, Insertion Sort, and Selection Sort ($O(n^2)$).
* **Heapsort**: Building a Heap (Top-Down vs. Bottom-Up) and the sorting algorithm ($O(n \log n)$).
* **Quicksort vs. Heapsort**: A detailed comparison of the two most popular $O(n \log n)$ sorting techniques.
* **Dynamic Order Statistics**: Finding the $i^{th}$ smallest element in a dynamic set.

### 2. Advanced Data Structures
* **K-Way Merge**: Efficiently merging $k$ sorted lists using a Min-Heap.
* **Hash Tables**: Implementation of Hash Tables with open addressing and quadratic probing to handle collisions.
* **Multi-way Trees**: Representing and traversing trees with more than two children.
* **Disjoint Sets**: Implementing Union-Find with path compression and union by rank optimizations.

### 3. Graph Algorithms
* **BFS (Breadth-First Search)**: Shortest path in unweighted graphs and layer-by-layer traversal.
* **DFS (Depth-First Search)**: Topological sorting and discovery/finishing time analysis.


> [!IMPORTANT]
> **Dependency Requirement:** The `Profiler.h` header file is essential for the execution of each project. It must be included in the source directory to enable performance measurement, constant counting, and the generation of data required for complexity charts.
---

## 📂 Project Structure
```text
├── 01-Sorting/            # Basic & Advanced sorting
├── 02-K-Way-Merge/        # Merging sorted lists
├── 03-Hash-Tables/        # Open addressing & Hashing
├── 04-Advanced-Trees/     # Multi-way trees & Order statistics
├── 05-Disjoint-Sets/      # Union-Find structures
├── 06-Graphs/             # BFS and DFS implementations
├── Analysis_reports/      # Screenshots with the plots
├── utils/                 # Profiler.h and analysis tools
└── README.md
