
/**
 * @file eval.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:10:38 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "object.h"

object sfs_eval( object );
int is_forme(object symbol);
int is_quote(object object);
int is_define( object object );
int is_set( object object );
int is_if( object object );
int is_and( object object );
int is_or( object object );

#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
