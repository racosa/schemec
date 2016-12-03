
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
#include "forme.h"
#include "primitive.h"

object sfs_eval( object input, object target_environment );
object sequential_eval(object arguments, object target_environment);


#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
