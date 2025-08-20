# Max-Profit Job Scheduling Solver

This project provides a C++ implementation for the **Weighted Job Scheduling Problem**. Given a set of jobs, each with a start time, end time, and an associated profit, the algorithm determines the subset of non-conflicting jobs that yields the maximum total profit.

The solution is based on dynamic programming and utilizes a sweep-line algorithm to efficiently process job start and end times as events. This approach achieves an optimal time complexity of O(n log n).

## Features

*   **Efficient Solution:** Solves the weighted job scheduling problem in O(n log n) time and O(n) space complexity.
*   **Dynamic Programming:** Employs a DP array where `dp[i]` stores the maximum profit achievable by a sequence of jobs ending with job `i`.
*   **Sweep-Line Algorithm:** Treats job start and end times as events on a timeline, processing them chronologically to find the maximum profit from previously completed, non-conflicting jobs.
*   **Flexible Interface:** Accepts input from both standard input (stdin) and command-line arguments, making it easy to test and integrate into scripts.
*   **Robust Input Validation:** Includes comprehensive checks to ensure input values (vector sizes, time ranges, profit ranges) adhere to predefined constraints.

## Prerequisites

*   A modern C++ compiler with support for C++11 or later (e.g., GCC/g++, Clang).

## Project Structure

The project is contained within a single source file, with a clear separation between the core algorithm and the program's entry point.

*   `jobScheduling(...)`: The core function that implements the dynamic programming and sweep-line algorithm. It is self-contained and operates on vector inputs.
*   `main(...)`: The main entry point responsible for:
    *   Parsing input from either stdin or command-line arguments.
    *   Validating the input against specified constraints.
    *   Calling the `jobScheduling` function and printing the final result to standard output.

## Compilation and Usage

To compile the source code, use a C++ compiler like `g++`.

```
g++ -std=c++11 -O2 -o scheduler main.cpp
```

The compiled program can be executed in two ways:

#### 1. Via Standard Input

Run the executable and provide the input directly in the console.

```
./scheduler
```

The program will wait for input in the following order:
1.  The number of jobs, `n`.
2.  `n` space-separated integers for the start times (`startTime`).
3.  `n` space-separated integers for the end times (`endTime`).
4.  `n` space-separated integers for the profits (`profit`).

**Example Input:**
```
4
1 2 3 3
3 4 5 6
50 10 40 70
```

**Expected Output:**
```
120
```
*(This corresponds to choosing the first job (profit 50) and the last job (profit 70).)*

#### 2. Via Command-Line Arguments

Provide all inputs as arguments directly on the command line.

**Format:** `./scheduler <n> <s_1> ... <s_n> <e_1> ... <e_n> <p_1> ... <p_n>`

**Example Command:**
```
./scheduler 4 1 2 3 3 3 4 5 6 50 10 40 70
```

**Expected Output:**
```
120
```

## Algorithm Logic Explained

The algorithm solves the problem by transforming it into a 1D dynamic programming problem, accelerated by a sweep-line approach.

1.  **Event Creation:** Each job is split into two `Event` objects: an "opening" event (start time) and a "closing" event (end time). Each event stores its time of occurrence and the corresponding `job_id`.

2.  **Sorting:** The `openings` and `closings` event vectors are sorted independently. They are sorted in descending order of time to allow for efficient O(1) processing using `vector::pop_back()`, which effectively processes events in chronological (ascending) order.

3.  **Dynamic Programming State:**
    *   A DP vector, `dp`, is created where `dp[i]` is defined as the maximum profit of a job sequence ending with job `i`.
    *   Initially, `dp[i]` is set to `profit[i]`, representing the profit from doing job `i` alone.

4.  **Sweep-Line Processing:**
    *   The algorithm iterates through the sorted `openings`. For each opening event of a job `j`, it first processes all `closing` events that occur before `j` starts.
    *   As it processes each closing event for a job `i`, it updates a variable `cur_max_profit`. This variable tracks the maximum profit found among all fully completed jobs (`dp[i]`) whose time intervals do not conflict with the current job `j`.
    *   Once all preceding closing events are handled, the algorithm considers the opening of job `j`. The DP value for `j` is updated: `dp[j] = profit[j] + cur_max_profit`. This signifies that the best way to include job `j` is to take its own profit plus the maximum profit from any valid, non-conflicting preceding job sequence.
    *   A `global_max_profit` variable is maintained throughout the sweep to store the maximum value ever seen in the `dp` array.

At the end of the process, `global_max_profit` holds the optimal solution to the problem.
