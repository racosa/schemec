
/**
 * @file read.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:30:11 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#ifndef _READ_H_
#define _READ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"

#define OPENING_PARENTHESIS     '('
#define CLOSING_PARENTHESIS    ')'
#define SPACE                ' '
#define END_OF_STRING        '\0'

enum {S_OK, S_KO, S_END};
enum {STATE_INIT, STATE_NUMBER, STATE_CHAINE_CHAR, STATE_CHAR, STATE_BOOLEAN, STATE_SYMBOL, STATE_EMPTY_LIST};

uint   sfs_get_sexpr( string input, FILE *fp );

object sfs_read( char *input, uint *here );
object sfs_read_atom( char *input, uint *here );
object sfs_read_pair( char *stream, uint *i );
void insert_object_in_tree(object car, object list);
void fix_quote_input(char *input, uint *here, uint atome_or_pair);

#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */
