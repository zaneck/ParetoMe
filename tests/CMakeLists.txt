function(add_list_test test_name source_file)
	add_executable(${test_name} ${source_file})
	target_link_libraries(${test_name} PRIVATE paretoMeCore)
	add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_list_test(test_list_insert_access test_list_insert_access.c)
add_list_test(test_list_remove_endpoints test_list_remove_endpoints.c)
add_list_test(test_list_remove_node test_list_remove_node.c)
add_list_test(test_list_toall_free test_list_toall_free.c)
add_list_test(test_list_alloc_fail test_list_alloc_fail.c)

add_executable(test_pareto test_pareto.c)
target_link_libraries(test_pareto PRIVATE paretoMeCore)
add_test(NAME test_pareto COMMAND test_pareto)

add_test(NAME test_main_usage_fail COMMAND $<TARGET_FILE:paretoMe>)
set_tests_properties(test_main_usage_fail PROPERTIES WILL_FAIL TRUE)

add_test(
	NAME test_main_usage_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_usage.sh $<TARGET_FILE:paretoMe>
)

add_test(
	NAME test_main_missing_file_fail
	COMMAND $<TARGET_FILE:paretoMe> 2 ${CMAKE_CURRENT_SOURCE_DIR}/data/does-not-exist.ssv
)
set_tests_properties(test_main_missing_file_fail PROPERTIES WILL_FAIL TRUE)

add_test(
	NAME test_main_missing_file_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_missing_file.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/does-not-exist.ssv
)

add_test(
	NAME test_main_run
	COMMAND $<TARGET_FILE:paretoMe> 2 ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)
set_tests_properties(test_main_run PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_test(
	NAME test_main_success_output
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_success.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)
set_tests_properties(test_main_success_output PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_test(
	NAME test_main_empty_input
	COMMAND $<TARGET_FILE:paretoMe> 2 ${CMAKE_CURRENT_SOURCE_DIR}/data/empty.ssv
)
set_tests_properties(test_main_empty_input PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_test(NAME test_main_output_exists COMMAND ${CMAKE_COMMAND} -E cat pareto.txt)
set_tests_properties(
	test_main_output_exists
	PROPERTIES
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
		DEPENDS test_main_run
)
