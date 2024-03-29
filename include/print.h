/**
 * @file print.c
 * @author Rafael Costa
 * @date Sat Nov 12 00:00:00 2016
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"


void sfs_print( object );

void sfs_print_atom( object );
void sfs_print_pair( object );

#ifdef __cplusplus
}
#endif

#endif /* _PRINT_H_ */
