#TARGET_LOGIN=root@targetdev
#TESTS_ENVIRONMENT=ssh $(TARGET_LOGIN) „cd ‘pwd’ ; „

# Tests should return 0 for success and !=0 for failure
OUR_SCRIPT_TESTS = check_spectra.sh
OUR_BINARY_TESTS = check_mfile

OUR_TESTS = $(OUR_BINARY_TESTS) $(OUR_SCRIPT_TESTS) 

TESTS = $(OUR_TESTS)

check_PROGRAMS = $(OUR_BINARY_TESTS)

check_mfile_SOURCES = check_mfile.c $(top_builddir)/src/mfile.h
#check_mfile_CFLAGS = @CHECK_CFLAGS@
#check_mfile_LDADD = @CHECK_LIBS@ $(top_builddir)/src/libmfile.la
check_mfile_LDADD = $(top_builddir)/src/libmfile.la

check_SCRIPTS = $(OUR_SCRIPT_TESTS)
noinst_SCRIPTS = $(OUR_SCRIPT_TESTS)
EXTRA_DIST = $(OUR_SCRIPT_TESTS) md5sums
