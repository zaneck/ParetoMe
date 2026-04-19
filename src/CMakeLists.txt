set(PARETOME_CORE_SRC_FILES_C pareto.c list.c)
set(PARETOME_SRC_FILES_H pareto.h list-local.h list.h)

add_library(paretoMeCore STATIC)
target_sources(paretoMeCore PRIVATE ${PARETOME_CORE_SRC_FILES_C} ${PARETOME_SRC_FILES_H})
target_include_directories(paretoMeCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(paretoMe)
target_sources(paretoMe PRIVATE main.c)
target_link_libraries(paretoMe PRIVATE paretoMeCore)

install(
	TARGETS paretoMe
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT "Runtime"
)
