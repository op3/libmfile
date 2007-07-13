/*
 * Copyright (C) 2007 Ralf Schulze <ralf.schulze@ikp.uni-koeln.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Include autoconf 'config.h' */
#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */


#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#ifdef DEBUG_OUTPUT
/* Define Debugging function (from: http://c-faq.com/cpp/debugmacs.html) */

static void debug(const char *, ...);
static void dbginfo(int, const char *);

#define DEBUG dbginfo(__LINE__, __FILE__), debug

static const char *dbgfile;
static int dbgline;

static void dbginfo(int line, const char *file) {
	dbgfile = file;
	dbgline = line;
}

static void debug(const char *fmt, ...) {
	va_list argp;
	fprintf(stderr, "DEBUG: (%s:%d):\t", dbgfile, dbgline);
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	fprintf(stderr, "\n");
}

#else

#define DEBUG

#endif /* DEBUG_OUTPUT */

#define PERROR fprintf(stderr, "(%s:%d)\t", \
		__FILE__,__LINE__),perror


#endif /* DEBUG_H_ */
