#
#	$Id: Makefile,v 1.23 1993/07/20 18:01:44 se Exp $
#
#	$Log: Makefile,v $
# Revision 1.23  1993/07/20  18:01:44  se
# added target 'ci' to check in all modified source files
#
# Revision 1.22  1992/12/31  11:01:56  se
# added target VMS_POSIX for posix environment on VAX VMS
#
# Revision 1.21  1992/08/27  08:37:21  se
# added target AIX for RS/6000 running AIX 3.2
#
# Revision 1.20  1992/07/30  13:30:29  se
# added back lost dependency on mate_minfo.c
#
# Revision 1.19  1992/07/30  12:49:17  se
# added dependency of install from $(LIBRARY) to 'all'
#
# Revision 1.16  1992/07/30  12:33:13  se
# added target .depend to automatically generate dependency info
#
# Revision 1.15  1992/07/30  12:28:12  se
# added dependencies on trixi_*.h
#
# Revision 1.14  1992/07/30  12:08:44  se
# added support for Convex C2
# added copyright note
#
# Revision 1.13  1992/07/08  21:13:04  se
# added reasonable additional warning options for use with GCC
#
# Revision 1.12  1992/06/30  14:55:59  se
# removed references to lc_c3.{c,h}
#
# Revision 1.11  1992/06/04  15:02:06  se
# added dependencies for txt_getput and txt_minfo
#
# Revision 1.10  1992/05/13  23:38:34  se
# added targets for txt_{minfo,getput}.{c,h} and adjusted dependencies
#
# Revision 1.9  1992/03/31  20:02:36  se
# *** empty log message ***
#
# Revision 1.8  1992/03/31  02:13:06  se
# dependencies corrected
#
# Revision 1.7  1992/03/31  01:53:10  se
# *** empty log message ***
#
# Revision 1.6  1992/03/17  15:10:18  se
# added makro COPT to seperate optimization from configuration flags
#
# Revision 1.5  1992/03/12  17:21:31  se
# added mate_* to dependency list
#
# Revision 1.4  1992/03/12  17:04:25  se
# added new source files mate_{getput,minfo}.{c,h} to handle the PC Mate
# file format.
#
# Revision 1.3  1992/02/18  23:29:37  se
# now defines VERIFY_COMPRESION
#
# Revision 1.2  1992/02/17  23:19:58  se
# new source files converters.{c,h}
#
# Revision 1.1  1992/02/13  14:20:54  se
# Initial revision
#

#
# libmfile.a:	Matrix access subroutines
#
# Copyright 1992 by
#	Stefan Esser
#	Institute for Nuclear Physics
#	University of Cologne, Germany
#


CC            = cc
COPTIMIZE     = -O
CDEBUG	      = -DVERIFY_COMPRESSION 

GCC	      = gcc
GCCDEBUG      = -g3 -DVERIFY_COMPRESSION 
GCCOPTIMIZE   = -O2 -funroll-loops -fschedule-insns \
			-fschedule-insns2 -finline-functions \
			-frerun-cse-after-loop \
			-Wcast-align -Wcast-qual -Wconversion \
			-Wshadow -Wtraditional -Wpointer-arith

#GCCOPTIMIZE    = -O2 -funroll-loops -fschedule-insns \
#			-fschedule-insns2 -finline-functions \
#			-frerun-cse-after-loop \
#			-Wall -Wshadow -Wtraditional -Wpointer-arith \
#			-Wcast-qual -Wcast-align -Wconversion \
#			-Wstrict-prototypes \
#			-Winline

CCFLAGS	      = $(COPTIMIZE) $(CDEBUG)
GCCFLAGS      = $(GCCOPTIMIZE) $(GCCDEBUG)

DEST	      = /ikp/local/lib

INCDEST       = /ikp/local/include/local/mfile

TARARCHIVE    = /vol/tar/mfile.tar.Z

MANPAGE       = mfile.3ikp

MANDIR        = /usr/local/man/man3

EXTHDRS	      = /usr/include/ansi_compat.h \
		/usr/include/ctype.h \
		/usr/include/errno.h \
		/usr/include/malloc.h \
		/usr/include/memory.h \
		/usr/include/stddef.h \
		/usr/include/stdio.h \
		/usr/include/stdlib.h \
		/usr/include/string.h \
		/usr/include/sys/file.h \
		/usr/include/sys/ipc.h \
		/usr/include/sys/shm.h \
		/usr/include/sys/stat.h \
		/usr/include/sys/types.h \
		/usr/include/unistd.h

HDRS	      = callindir.h \
		converters.h \
		disk_access.h \
		endian.h \
		getputint.h \
		lc_c1.h \
		lc_c2.h \
		lc_getput.h \
		lc_minfo.h \
		maccess.h \
		mat_types.h \
		mate_getput.h \
		mate_minfo.h \
		mfile.h \
		mopen.h \
		oldmat_getput.h \
		oldmat_minfo.h \
		shm_access.h \
		shm_getput.h \
		shm_minfo.h \
		trixi_getput.h \
		trixi_minfo.h \
		txt_getput.h \
		txt_minfo.h

PUBHDRS       = mfile.h

LIBRARY	      = libmfile.a

LIBRARY_U     = libmfile.b

MAKEFILE      = Makefile

OBJS	      = callindir.o \
		converters.o \
		disk_access.o \
		getputint.o \
		lc_c1.o \
		lc_c2.o \
		lc_getput.o \
		lc_minfo.o \
		maccess.o \
		mat_types.o \
		mate_getput.o \
		mate_minfo.o \
		minfo.o \
		mopen.o \
		oldmat_getput.o \
		oldmat_minfo.o \
		shm_access.o \
		shm_getput.o \
		shm_minfo.o \
		specio.o \
		trixi_getput.o \
		trixi_minfo.o \
		txt_getput.o \
		txt_minfo.o

PRINT	      = pr

SRCS	      = callindir.c \
		converters.c \
		disk_access.c \
		getputint.c \
		lc_c1.c \
		lc_c2.c \
		lc_getput.c \
		lc_minfo.c \
		maccess.c \
		mat_types.c \
		mate_getput.c \
		mate_minfo.c \
		minfo.c \
		mopen.c \
		oldmat_getput.c \
		oldmat_minfo.c \
		shm_access.c \
		shm_getput.c \
		shm_minfo.c \
		specio.c \
		trixi_getput.c \
		trixi_minfo.c \
		txt_getput.c \
		txt_minfo.c

all:		.depend
		@make `uname || arch`

# Ultrix 4.2
ULTRIX:
		make $(LIBRARY) CC=$(GCC) CFLAGS='$(GCCFLAGS)'

# SunOS 4.1.1
SunOS:
		make $(LIBRARY) CC=$(GCC) CFLAGS='$(GCCFLAGS)'

# Linux
Linux:
		make $(LIBRARY) CC=$(GCC) CFLAGS='$(GCCFLAGS)'

# RS/6000, AIX 3.2
AIX:
		make $(LIBRARY) CFLAGS='$(CCFLAGS) -D_ALL_SOURCE'

# HP 9000/7xx, HP 9000/8xx, HP-UX 7.0
HP-UX:
		make $(LIBRARY) CFLAGS='$(CCFLAGS) -Aa -D_HPUX_SOURCE'

# Convex
c2mp:		
		make $(LIBRARY) CC=cc CFLAGS='$(CCFLAGS)'

# VMS_POSIX
VMS_POSIX:
		make $(LIBRARY) CFLAGS='$(CCFLAGS) -DNO_SHM -Du_int=unsigned -Du_short="unsigned short" -Du_char="unsigned char"'




$(LIBRARY):	$(OBJS) $(HDRS) $(SRCS)
		ar -rcu $(LIBRARY) $(OBJS)
		-ranlib $(LIBRARY)

clean:;		rm -f $(OBJS)

realclean:;	rm -f $(OBJS) $(SRCS) $(HDRS) $(LIBRARY)

#.depend:	Makefile $(SRCS) $(HDRS)
#		@-make co
#		-mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)
#		touch .depend

.depend:	Makefile $(SRCS) $(HDRS)
		-mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)
		touch .depend

extract:;	ar xo $(DEST)/$(LIBRARY)
		@rm -f __.SYMDEF

index:;		ctags -wx $(HDRS) $(SRCS)

install:	install.lib install.man install.inc

install.lib:	$(DEST)/$(LIBRARY)

install.man:	$(MANDIR)/$(MANPAGE)

install.inc:	$(INCDEST)/$(PUBHDRS)

$(DEST)/$(LIBRARY):	all
		mkdir -p $(DEST)
		-rm -f $(DEST)/$(LIBRARY)
		cp $(LIBRARY) $(DEST)/$(LIBRARY)
		ranlib $(DEST)/$(LIBRARY)

$(MANDIR)/$(MANPAGE):	$(MANPAGE)
		cp $(MANPAGE) $(MANDIR)

$(INCDEST)/$(PUBHDRS):	$(PUBHDRS)
		mkdir -p $(INCDEST)
		cp $(PUBHDRS) $(INCDEST)
		cd $(INCDEST)/.. ; ln -s mfile/mfile.h ; cd -

library:        $(LIBRARY)

print:		$(PRINT) $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); ctags $(HDRS) $(SRCS)

tar:		$(TARARCHIVE)

$(TARARCHIVE):	$(SRCS) $(HDRS) $(MANPAGE) Makefile
		tar czfhv $(TARARCHIVE) $(SRCS) $(HDRS) $(MANPAGE) RCS/* Makefile TODO ChangeLog

test:		$(LIBRARY)
		$(CC) -o test $(CFLAGS) -DTEST mopen.c $(LIBRARY)

ci:		$(SRCS) $(HDRS) Makefile
		for f in $?; do rcsdiff $$f || ci -u -q $$f; done

co:		$(SRCS) $(HDRS) Makefile

update:         $(DEST)/$(LIBRARY)

#$(DEST)/$(LIBRARY): $(SRCS) $(HDRS) $(EXTHDRS)
#		@-ar xo $(DEST)/$(LIBRARY)
#		@make -f $(MAKEFILE) DEST=$(DEST) install clean
###
