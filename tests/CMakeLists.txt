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
	NAME test_main_option_success_output
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_success.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)
set_tests_properties(test_main_option_success_output PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_test(
	NAME test_main_option_invalid_dim_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_invalid_dim.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)

add_test(
	NAME test_main_option_invalid_flag_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_invalid_flag.sh $<TARGET_FILE:paretoMe>
)

add_test(
	NAME test_main_option_help_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_help_message.sh $<TARGET_FILE:paretoMe>
)

add_test(
	NAME test_main_option_extra_arg_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_extra_arg_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)

add_test(
	NAME test_main_option_missing_file_arg_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_missing_file_arg_message.sh $<TARGET_FILE:paretoMe>
)

add_test(
	NAME test_main_option_zero_dim_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_zero_dim_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)

add_test(
	NAME test_main_option_overflow_dim_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_option_overflow_dim_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/sample.ssv
)

add_test(
	NAME test_main_invalid_token_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_invalid_input_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/invalid_token.ssv
)

add_test(
	NAME test_main_short_row_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_invalid_input_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/short_row.ssv
)

add_test(
	NAME test_main_extra_value_message
	COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_main_invalid_input_message.sh $<TARGET_FILE:paretoMe> ${CMAKE_CURRENT_SOURCE_DIR}/data/extra_value.ssv
)

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
