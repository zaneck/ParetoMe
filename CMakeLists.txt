cmake_minimum_required(VERSION 3.16)

project(paretoMe VERSION 0.0.1 LANGUAGES C)

enable_testing()

option(ENABLE_COVERAGE "Build with coverage flags" OFF)

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

if(ENABLE_COVERAGE)
	find_program(GCOVR_EXECUTABLE gcovr)
	if(GCOVR_EXECUTABLE)
		add_custom_target(coverage
			COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/tests/CMakeFiles/test_list.dir
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
