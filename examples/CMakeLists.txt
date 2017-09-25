project('flex_example')

set(MNAME hello)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/${MNAME}.lex)
set(LEX ${MNAME}.lex.c)

add_custom_command(OUTPUT ${LEX}
    COMMAND flex -o ${LEX} ${SRC}
    DEPENDS ${SRC}
)

add_executable(${MNAME} ${LEX})
target_link_libraries(${MNAME} libfl.so)
