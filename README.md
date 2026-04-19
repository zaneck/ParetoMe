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
rm -rf build build-coverage
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
ctest --test-dir build --output-on-failure
```

## Coverage

Coverage reports are generated with `gcovr`.

```sh
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage
cmake --build build-coverage --target coverage
```

Reports are generated in:

- `build-coverage/coverage.xml`
- `build-coverage/coverage.html`

## Benchmark

Benchmark the main CLI with a generated deterministic dataset using `hyperfine`:

```sh
cmake -S . -B build
cmake --build build --target benchmark_main
```

Tune the benchmark workload at configure time if needed:

```sh
cmake -S . -B build \
	-DPARETOME_BENCHMARK_DIMS=10,200,300,400,500 \
	-DPARETOME_BENCHMARK_ROWS=10000 \
	-DPARETOME_BENCHMARK_ROW_MULTIPLIERS=1,2,3,4,5 \
	-DPARETOME_BENCHMARK_REPETITIONS=10 \
	-DPARETOME_BENCHMARK_WARMUP=3
cmake --build build --target benchmark_main
```

Generate a gnuplot graph from benchmark results:

```sh
cmake --build build --target benchmark_graph
```

Benchmark artifacts are generated in:

- `build/benchmarks/main/benchmark_data.txt` (parsed data)
- `build/benchmarks/main/benchmark_results.png` (gnuplot graph)
