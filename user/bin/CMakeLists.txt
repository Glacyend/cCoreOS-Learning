set(USER_PROGRAMS
    00_power_3
    01_power_5
    02_power_7
    03_sleep
    04_load_fault
    05_store_fault
    06_sbrk_test
)

set(USER_LINKER ${CMAKE_CURRENT_SOURCE_DIR}/../linker.ld)

foreach(prog ${USER_PROGRAMS})
    add_executable(${prog} ${prog}.c)
    target_link_libraries(${prog} PRIVATE user)

    target_link_options(${prog} PRIVATE -T ${USER_LINKER})

    add_custom_command(TARGET ${prog} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${prog}> $<TARGET_FILE_DIR:${prog}>/${prog}.bin
        COMMENT "objcopy ${prog} -> ${prog}.bin"
    )
endforeach()
