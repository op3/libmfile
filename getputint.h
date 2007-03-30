/*
 * getputint.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>

/* $Id: getputint.h,v 1.8 1993/07/20 18:00:53 se Exp $
 *
 * $Log: getputint.h,v $
 * Revision 1.8  1993/07/20  18:00:53  se
 * modified for use with access method module
 *
 * Revision 1.7  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.6  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.5  92/07/08  06:21:53  06:21:53  se (Stefan Esser)
 * changed 2nd parameter to get and put from char* to void* and introduced
 * local char *buffer for actual accesses
 * 
 * Revision 1.4  1992/05/13  23:29:46  se
 * changed second parameter of get and put from void * to char *
 *
 * Revision 1.3  1992/03/31  01:55:44  se
 * changed type of buffer from char* to void*
 * added definitions of {get,put}{le,he}8()
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/*
int   get    P_((int fd, void *buffer, u_int pos, u_int num));
int   put    P_((int fd, void *buffer, u_int pos, u_int num));
*/

/* signed low endian 8 byte matrix file */
u_int getle8 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle8 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 8 byte matrix file */
u_int gethe8 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe8 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed low endian 4 byte matrix file */
u_int getle4 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle4 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 4 byte matrix file */
u_int gethe4 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe4 P_((amp ap, int *buffer, u_int pos, u_int num));

/* unsigned low endian 2 byte matrix file */
u_int getle2 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle2 P_((amp ap, int *buffer, u_int pos, u_int num));

/* unsigned high endian 2 byte matrix file */
u_int gethe2 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe2 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed low endian 2 byte matrix file */
u_int getle2s P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 2 byte matrix file */
u_int gethe2s P_((amp ap, int *buffer, u_int pos, u_int num));
