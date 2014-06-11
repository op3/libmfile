cmake_minimum_required( VERSION 2.8 )
enable_language( C )

add_executable(check_mfile check_mfile.c)
target_link_libraries(check_mfile mfile)
add_dependencies(check_mfile mfile)

add_test(run_check_mfile check_mfile)
add_test(run_check_spectra md5sum -c ${CMAKE_SOURCE_DIR}/test/md5sums)
