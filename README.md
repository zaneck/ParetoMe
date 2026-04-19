# ParetoMe
Just a filter for space separated value file.

## Build

```sh
cmake -S . -B build
cmake --build build
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
