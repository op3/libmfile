cmake_minimum_required( VERSION 2.6 )

enable_language( C )

set( PROJ_NAME "mfile" )
set( PROJ_INCLUDES "src" )
file( GLOB_RECURSE PROJ_SOURCES src/*.c )


add_definitions("-std=gnu99 -O3 -ftrapv -DNO_SHM")
#### Explanations for compiler flags
# -std=gnu99 -> Use the gnu version of the c99 standard
# -O3 -> best optimisation
# -Wall -pedantic -Wcast-align -Wcast-qual -Wconversion -Wshadow -Wpointer-arith
# -ftrapv -> will cause the program to abort on signed integer overflow (formally "undefined behaviour" in C)
#### Other useful flags
# -g -> Produce debugging information in the operating system's native format
#### Flags that were set in the original versions but are no longer state-of-the-art
# -funroll-loops -> GCC manual says that using -funroll-loops makes code larger and run more slowly
# -finline-functions -> redundant with -O3
# -frerun-cse-after-loop -> redundant with -O3
# -DNO_SHM -> ???
# -MT -MD -MP -MF -> ???



#### Replacment for config.h 

# Check endian-ness
include( TestBigEndian )
test_big_endian( BIGENDIAN )
if( NOT ${BIGENDIAN} )
	add_definitions( -DLOWENDIAN )
endif( NOT ${BIGENDIAN} )

# Check for sprintf
include( CheckFunctionExists ) 
check_function_exists( snprintf HAVE_SNPRINTF ) 
if( ${HAVE_SNPRINTF} )
	add_definitions( -DHAVE_SNPRINTF )
endif( ${HAVE_SNPRINTF} )



project( ${PROJ_NAME} )
include_directories( ${PROJ_INCLUDES} )
add_library( ${PROJ_NAME} SHARED ${PROJ_SOURCES} )

install( TARGETS ${PROJ_NAME} DESTINATION lib )
install( FILES "${PROJ_INCLUDES}/mfile.h" DESTINATION include )

enable_testing()
add_subdirectory( test )
