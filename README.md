# ParetoMe
Just a filter for space separated value file.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Clean

Remove all generated build and benchmark artifacts:

```sh
cmake -E rm -rf build build-coverage
```

## Run

Run the executable from the project root with: `paretoMe <dim> <file>`

```sh
./build/src/paretoMe 2 ./path/to/data.ssv
```

## Install

Install to the default prefix:

```sh
cmake --install build
```

Install to a custom prefix:

```sh
cmake --install build --prefix /usr/local
```

## Test

The suite covers list operations, Pareto archive behavior, and CLI integration cases (usage failure, missing input file, valid input, empty input, and output file generation).

```sh
cmake --build build --target test
```

## Coverage

Coverage reports are generated with `gcovr`.

```sh
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage
cmake --build build-coverage --target test
cmake --build build-coverage --target coverage
```

Reports are generated in:

- `build-coverage/coverage.xml`
- `build-coverage/coverage.html`

## Benchmark

Use `hyperfine` through CMake targets to benchmark the CLI.

Prerequisites:

- `hyperfine` for `benchmark_main`
- `python3` and `gnuplot` for `benchmark_graph`

Quick start:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target benchmark_main
```

For best local performance numbers, you can also enable native tuning and LTO:

```sh
cmake -S . -B build \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_FLAGS_RELEASE="-O3 -march=native -flto"
cmake --build build --target benchmark_main
```

This is often the easiest 1.5x to 3x speedup before algorithmic changes.

You can customize benchmark size and runtime at configure time:

```sh
cmake -S . -B build \
	-DCMAKE_BUILD_TYPE=Release \
	-DPARETOME_BENCHMARK_DIMS=10,200,300,400,500 \
	-DPARETOME_BENCHMARK_ROWS=10000 \
	-DPARETOME_BENCHMARK_ROW_MULTIPLIERS=1,2,3,4,5 \
	-DPARETOME_BENCHMARK_REPETITIONS=10 \
	-DPARETOME_BENCHMARK_WARMUP=3
cmake --build build --target benchmark_main
```

To generate the benchmark chart:

```sh
cmake --build build --target benchmark_main
cmake --build build --target benchmark_graph
```

Generated artifacts:

- `build/benchmarks/main/results.json`
- `build/benchmarks/main/summary.txt`
- `build/benchmarks/main/summary.md`
- `build/benchmarks/main/benchmark_data.txt`
- `build/benchmarks/main/benchmark_results.png`
