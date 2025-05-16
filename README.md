# OpenMP Parallelism Benchmark

This project benchmarks and visualizes the performance gain (speedup) from using OpenMP for parallel processing in C++. It compares serial and parallel versions of an average computation on a large data buffer, and plots the speedup as the number of threads increases.

## Contents

- `main.cpp`: Core C++ program that calculates the average of a large array either serially or in parallel using OpenMP.
- `Makefile`: Builds different versions of the executable (serial, race condition, and correct parallel).
- `run.py`: Python script that automates the execution of the C++ binaries, collects timing data, and plots speedup vs. thread count.

## Requirements

### C++ Build Tools

- GCC with OpenMP support (e.g., `g++ -fopenmp`)
- Make

### Python

- Python 3.8+
- `matplotlib` for plotting
- `os` for path management

## Building the Project

Use the Makefile to build three versions of the program:

```bash
make all
```

This will produce:

- `serial.exe`: Serial implementation.
- `race.exe`: Parallel implementation with potential race conditions.
- `parallel.exe`: Correct parallel implementation using `#pragma omp critical`.

## Running the Benchmark

Run the Python script:

```bash
python run.py
```

This script will:

- Run the serial version to get a baseline time.
- Execute the parallel version using 1 to 12 threads.
- Compute and print the speedup.
- Plot a graph: **Speedup vs. Number of Threads**.

## Notes

- The buffer size is set to 20 million elements by default. You can adjust this by modifying the `BufferSize` variable in `run.py`.
- The correct average is verified by comparing with the serial version.
- The `race.exe` version can be used to demonstrate incorrect results due to data races (when `#pragma omp critical` is not used).

## License

This project is provided for educational and benchmarking purposes. No explicit license is applied.
