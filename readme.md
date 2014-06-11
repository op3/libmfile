# mfile

[Name](#name) |
 [Syntax](#syntax) |
 [Description](#description) |
 [Example](#example) |
 [Restrictions](#restrictions) |
 [Author](#author)

* * * * *

## <a name="name"></a>Name
mopen, mclose, mgetint, mgetflt, mgetdbl, mputint, mputflt, mputdbl,
mgetfmt, msetfmt

## <a name="syntax"></a>Syntax

``` c
#include "mfile.h"

MFILE *mopen(name, mode)
char *name
char *mode

int mclose(mat)
MFILE *mat

int mgetint(mat, buffer, level, line, column, num)
MFILE *mat
int *buffer
int level, line, column, num

int mgetflt (mat, buffer, level, line, column, num)
MFILE *mat
float *buffer
int level, line, column, num

int mgetdbl (mat, buffer, level, line, column, num)
MFILE *mat
double *buffer
int level, line, column, num

int mputint (mat, buffer, level, line, column, num)
MFILE *mat
int *buffer
int level, line, column, num

int mputflt (mat, buffer, level, line, column, num)
MFILE *mat
float *buffer
int level, line, column, num

int mputdbl (mat, buffer, level, line, column, num)
MFILE *mat
double *buffer
int level, line, column, num

int mgetinfo (mat, info)
MFILE *mat
minfo *info

int msetinfo (mat, info)
MFILE *mat
minfo *info

int mgetfmt (mat, format)
MFILE *mat
char *format

int msetfmt (mat, format)
MFILE *mat
char *format

int load_spec (name, buffer, num)
char *name
int *buf
int num

int save_spec (name, buffer, num)
char *name
int *buf
int num
```

## <a name="description"></a>Description

These functions allow machine independent and efficient access to
coincidence matrices. Traditional matrices (long and short, VAX and
HighEndian) can be read and written, but some special formats are
supported, which reduce the storage requirements by a factor of 3 to 10.
The main goal was to provide an abstract interface for simple access to
matrix files, independent of their format, and to make it possible to
use these programs with new formats by simply relinking them with an
extended version of the library libmat.a.

The subroutine opens a matrix file named *name* for reading and/or
writing as specified by *mode* and returns a pointer to a struct MFILE
on success or NULL in case of an error. Currently defined values of
*mode* are "r" for read only "w" for write only and "r+" for read write
access.

The subroutine closes the matrix file and returns 0 on success, −1 on
failure.

The subroutine reads *num* matrix elements starting at level *level*,
line *line*, and column *column* from the previously opened matrix file
specified by *mat* into the array of long integers *buffer*. It returns
the number of elements actually read on success or −1 in case of a fatal
error.

The subroutine writes *num* elements into the matrix file. Parameters
and return codes are the same as returned by *mget*.

The `mgetinfo` subroutine

The `msetinfo` subroutine

The `mgetfmt` subroutine

The `msetfmt` subroutine

The `load_spec` subroutine

The `save_spec` subroutine

## <a name="Syntax"></a>Example


The following code fragment lists a program that opens the matrix file
specified by the first argument for reading and creates a new matrix
under the name given as the second argument. Only marginal error
checking is shown to keep the example short.

``` c
main (argc, argv)
int argc;
char **argv;
{
    MFILE *src, *dst;
    minfo info;
    int levels, lines, columns;
    int lev, lin;

/* open both matrix files */
    src = mopen (argv[1], "r");
    dst = mopen (argc[2], "w");

/* abort if open failed */
    if (!src || !dst) exit (1);

/* get matrix dimensions */
    mgetinfo (src, &info);
    levels  = info.levels;
    lines   = info.lines;
    columns = info.columns;

/* set dest matrix dims equal to src dims */
    mgetinfo (dst, &info);
    info.levels  = levels;
    info.lines   = lines;
    info.columns = columns;
    if (msetinfo (dst, &info) != 0) exit (1);

/* allocate line buffer */
    buffer = (int *) malloc (c * sizeof(int));

/* copy matrix line by line */
    for (lev = 0; lev < levels; lev++) {
        for (lin = 0; lin < lines; lin++) {
            mgetint (src, buffer, lev, lin, 0, columns);
            mputint (dst, buffer, lev, lin, 0, columns);
        }
    }

/* close matrices */
    mclose (src);
    if (mclose (dst) != 0) exit (1);
    exit (0);
}
```

## <a name="restrictions"></a>Restrictions

Traditional style matrizes and spectra may have at most 16384 columns.

## <a name="author"></a>Author

Stefan Esser <se@ikp.uni-koeln.de>

Ralf Schulze <r.schulze@ikp.uni-koeln.de>

Nigel Warr <n.warr@ikp.uni-koeln.de>

Jan Mayer <jan.mayer@ikp.uni-koeln.de>

Institut of Nuclear Physics, Cologne, Germany
