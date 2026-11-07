set(USER_PROGRAMS
    00_hello
    01_power_7
    02_power_5
    03_power_13
)

set(USER_LINKER ${CMAKE_CURRENT_SOURCE_DIR}/../linker.ld)
file(READ ${USER_LINKER} USER_LINKER_TEMPLATE)

set(APP_IDX 0)
foreach(prog ${USER_PROGRAMS})
    add_executable(${prog} ${prog}.c)
    target_link_libraries(${prog} PRIVATE user)

    math(EXPR APP_ADDR "0x80400000 + ${APP_IDX} * 0x20000")
    string(REPLACE "0x80400000" "${APP_ADDR}" USER_LINKER_CONTENT "${USER_LINKER_TEMPLATE}")
    set(USER_LINKER_PROG ${CMAKE_CURRENT_BINARY_DIR}/${prog}.ld)
    file(WRITE ${USER_LINKER_PROG} "${USER_LINKER_CONTENT}")

    target_link_options(${prog} PRIVATE -T ${USER_LINKER_PROG})

    add_custom_command(TARGET ${prog} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${prog}> $<TARGET_FILE_DIR:${prog}>/${prog}.bin
        COMMENT "objcopy ${prog} -> ${prog}.bin"
    )

    math(EXPR APP_IDX "${APP_IDX} + 1")
endforeach()
