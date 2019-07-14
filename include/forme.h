/**
 * @file forme.h
 * @author Rafael Costa
 * @date Sat Dec 03 21:05:43 2016
 * @brief Forme definition for the schemeC interpreter.
 *
 * Formes definition for the schemeC interpreter.
 */

#ifndef _FORME_H_
#define _FORME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environment.h"

void initialize_formes( void );
int is_forme(object symbol);
int is_quote(object object);
int is_define( object object );
int is_set( object object );
int is_if( object object );
int is_and( object object );
int is_or( object object );
int is_primitive( object object );
int is_begin( object object );
int is_lambda( object object );
int is_compound( object object );
int is_let( object object );
int is_let_asterisc( object object );
int is_map( object object );

#ifdef __cplusplus
}
#endif
#endif /*_FORME_H_*/
