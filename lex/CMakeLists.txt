project('miniC-lex')

set(MNAME hello)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/miniC.lex)
set(YSRC ${CMAKE_CURRENT_SOURCE_DIR}/miniC.y)
set(LEXC ${CMAKE_CURRENT_BINARY_DIR}/miniC.lex.c)
set(YACCH ${CMAKE_CURRENT_BINARY_DIR}/miniC.yacc.c)
set(EXEC miniC-lex)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(MiniCLYsource DEPENDS ${SRC} ${YSRC})
add_custom_command(TARGET MiniCLYsource 
    OUTPUTS ${LEXC}
    COMMAND flex -o ${LEXC} ${SRC}
)

add_custom_command(TARGET MiniCLYsource 
    OUTPUTS ${YACCH}
    COMMAND yacc -o ${YACCH} ${YSRC}
)

SET_SOURCE_FILES_PROPERTIES(${LEXC} ${YACCH} GENERATED)

add_executable(${EXEC} ${LEXC})
#set_property(TARGET ${EXECOB} PROPERTY LINK_DEPENDS ${YACCH})
add_dependencies(${EXEC} MiniCLYsource)
target_link_libraries(${EXEC} libfl.so)
