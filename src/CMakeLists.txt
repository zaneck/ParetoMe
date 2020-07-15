set(PARETOME_SRC_FILES_C main.c pareto.c list.c)
set(PARETOME_SRC_FILES_H pareto.h list-local.h list.h)
set(PARETOME_SRC_FILES ${PARETOME_SRC_FILES_H} ${PARETOME_SRC_FILES_C})

add_executable(paretoMe ${PARETOME_SRC_FILES})

install(
	TARGETS paretoMe
	RUNTIME DESTINATION bin COMPONENT "Runtime"
)
