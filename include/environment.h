/**
 * @file environment.h
 * @author Rafael Costa
 * @date Sat Oct 16 00:56:33 2016
 * @brief Environment definition for the schemeC interpreter.
 *
 * Environment definition for the schemeC interpreter.
 */

#ifndef _ENVIRONMENT_H_
#define  _ENVIRONMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

extern object top_level_environment;
/*extern object current_environment;
*/
void make_top_level_environment( void );
object insert_symbol_in_environment( object symbol_pair, object target_environment );
object search_symbol_in_environment( string symbol, object target_environment, int search_in_all_environments );
object make_environment ( object new_environment, object target_environment );

#ifdef __cplusplus
}
#endif

#endif /* _ENVIRONMENT_H_ */
