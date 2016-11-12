/**
 * @file environment.h
 * @author Rafael Costa Sales
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

extern object environment;

object make_top_level_environment( void );
object insert_symbol_in_environment( object symbol_pair );
object search_symbol_in_environment( string symbol );
void initialize_formes( void );

#ifdef __cplusplus
}
#endif

#endif /* _ENVIRONMENT_H_ */
