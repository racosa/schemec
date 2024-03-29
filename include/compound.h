/**
 * @file compound.h
 * @author Rafael Costa
 * @date Sun Nov 27 21:05:43 2016
 * @brief Compounds definition for the schemec interpreter.
 *
 * Compounds definition for the schemec interpreter.
 */

#ifndef _COMPOUND_H_
#define _COMPOUND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environment.h"

int bind_compound_arguments (object anonymous_function, object arguments, object bind_environment, int search_in_all_environments);
object eval_argument_list(object values, object environment);
object define_function(object function, object function_symbol, object parameters, object body,
  object target_environment, object new_environment);

#ifdef __cplusplus
}
#endif
#endif /*_COMPOUND_H_*/
