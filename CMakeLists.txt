cmake_minimum_required(VERSION 3.16)

project(paretoMe VERSION 0.0.1 LANGUAGES C)

enable_testing()

option(ENABLE_COVERAGE "Build with coverage flags" OFF)
set(PARETOME_BENCHMARK_DIMS "10,200,300,400,500" CACHE STRING "Comma-separated benchmark input dimensionalities")
set(PARETOME_BENCHMARK_ROWS 10000 CACHE STRING "Base row count for benchmark input generation")
set(PARETOME_BENCHMARK_ROW_MULTIPLIERS "1,2,3,4,5" CACHE STRING "Comma-separated row multipliers applied to the benchmark row base")
set(PARETOME_BENCHMARK_REPETITIONS 5 CACHE STRING "Number of benchmark repetitions")
set(PARETOME_BENCHMARK_WARMUP 2 CACHE STRING "Number of benchmark warmup runs")

if(ENABLE_COVERAGE)
	if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
		add_compile_options(-O0 -g --coverage)
		add_link_options(--coverage)
	else()
		message(WARNING "Coverage requested, but compiler does not support --coverage flags")
	endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(tests)

find_program(HYPERFINE_EXECUTABLE hyperfine)
if(HYPERFINE_EXECUTABLE)
	add_custom_target(benchmark_main
		COMMAND /bin/sh ${CMAKE_SOURCE_DIR}/benchmarks/run_main_benchmark.sh
			${HYPERFINE_EXECUTABLE}
			$<TARGET_FILE:paretoMe>
			${CMAKE_BINARY_DIR}/benchmarks/main
			${PARETOME_BENCHMARK_DIMS}
			${PARETOME_BENCHMARK_ROWS}
			${PARETOME_BENCHMARK_ROW_MULTIPLIERS}
			${PARETOME_BENCHMARK_REPETITIONS}
			${PARETOME_BENCHMARK_WARMUP}
		DEPENDS paretoMe
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Benchmark paretoMe main CLI"
		VERBATIM
	)
	
	# Graph generation target
	find_program(PYTHON3_EXECUTABLE python3)
	if(PYTHON3_EXECUTABLE)
		add_custom_target(benchmark_graph
			COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/benchmarks/generate_graph.py
				${CMAKE_BINARY_DIR}/benchmarks/main/results.json
				${CMAKE_BINARY_DIR}/benchmarks/main
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Generate gnuplot graph from benchmark results"
			VERBATIM
		)
	else()
		message(STATUS "python3 not found: benchmark_graph target will not be available")
	endif()
else()
	message(STATUS "hyperfine not found: benchmark_main target will not be available")
endif()

if(ENABLE_COVERAGE)
	find_program(GCOVR_EXECUTABLE gcovr)
	if(GCOVR_EXECUTABLE)
		add_custom_target(coverage
			COMMAND /bin/sh -c "find '${CMAKE_BINARY_DIR}' -name '*.gcda' -delete"
			COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
			COMMAND ${GCOVR_EXECUTABLE}
							--root ${CMAKE_SOURCE_DIR}
							--filter ${CMAKE_SOURCE_DIR}/src
							--exclude ${CMAKE_SOURCE_DIR}/tests
							--exclude-unreachable-branches
							--print-summary
							--xml-pretty
							--output ${CMAKE_BINARY_DIR}/coverage.xml
							--html-details ${CMAKE_BINARY_DIR}/coverage.html
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Run tests and generate coverage reports (coverage.xml, coverage.html)"
			VERBATIM
		)
	else()
		message(STATUS "gcovr not found: coverage target will not be available")
	endif()
endif()
