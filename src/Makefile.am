AM_CFLAGS= -O2 -g -Wall -pedantic \
	-funroll-loops -fschedule-insns \
	-fschedule-insns2 -finline-functions \
	-frerun-cse-after-loop \
	-Wcast-align -Wcast-qual -Wconversion \
	-Wshadow -Wtraditional -Wpointer-arith

lib_LTLIBRARIES = libmfile.la

libmfile_la_SOURCES = mfile.h \
		callindir.c \
		callindir.h \
		converters.c \
		converters.h \
		disk_access.c \
		disk_access.h \
		endian.h \
		getputint.c \
		getputint.h \
		lc_c1.c \
		lc_c1.h \
		lc_c2.c \
		lc_c2.h \
		lc_getput.c \
		lc_getput.h \
		lc_minfo.c \
		lc_minfo.h \
		maccess.c \
		maccess.h \
		mate_getput.c \
		mate_getput.h \
		mate_minfo.c \
		mate_minfo.h \
		mat_types.c \
		mat_types.h \
		minfo.c \
		mopen.c \
		mopen.h \
		oldmat_getput.c \
		oldmat_getput.h \
		oldmat_minfo.c \
		oldmat_minfo.h \
		shm_access.c \
		shm_access.h \
		shm_getput.c \
		shm_getput.h \
		shm_minfo.c \
		shm_minfo.h \
		specio.c \
		trixi_getput.c \
		trixi_getput.h \
		trixi_minfo.c \
		trixi_minfo.h \
		txt_getput.c \
		txt_getput.h \
		txt_minfo.c \
		txt_minfo.h 

# pkginclude_HEADERS = mfile.h

# Headers that will be installed to filesystem
include_HEADERS 	= mfile.h 

libmfile_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

datarootdir = @datarootdir@
