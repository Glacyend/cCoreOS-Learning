set(USER_PROGRAMS
    00_hello
)

set(USER_LINKER ${CMAKE_CURRENT_SOURCE_DIR}/../linker.ld)

set(APP_IDX 0)
foreach(prog ${USER_PROGRAMS})
    add_executable(${prog} ${prog}.c)
    target_link_libraries(${prog} PRIVATE user)

    math(EXPR APP_ADDR "0x80400000 + ${APP_IDX} * 0x20000")
    target_link_options(${prog} PRIVATE -T ${USER_LINKER} -Wl,--defsym=BASE_ADDRESS=${APP_ADDR})

    add_custom_command(TARGET ${prog} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${prog}> $<TARGET_FILE_DIR:${prog}>/${prog}.bin
        COMMENT "objcopy ${prog} -> ${prog}.bin"
    )

    math(EXPR APP_IDX "${APP_IDX} + 1")
endforeach()
