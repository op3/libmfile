cmake_minimum_required( VERSION 2.6 )
enable_language( C )

add_executable(check_mfile check_mfile.c)
target_link_libraries(check_mfile mfile)
add_dependencies(check_mfile mfile)

if(WIN32)
add_custom_command(TARGET check_mfile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_BINARY_DIR}/../libmfile.dll"
    "${CMAKE_CURRENT_BINARY_DIR}/libmfile.dll")
endif(WIN32)

add_test(run_check_mfile check_mfile)
add_test(run_check_spectra md5sum -c ${CMAKE_SOURCE_DIR}/test/md5sums)
