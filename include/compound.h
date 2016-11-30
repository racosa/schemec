/**
 * @file compound.h
 * @author Rafael Costa Sales
 * @date Sun Nov 27 21:05:43 2016
 * @brief Compounds definition for the schemeC interpreter.
 *
 * Compounds definition for the schemeC interpreter.
 */

#ifndef _COMPOUND_H_
#define _COMPOUND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environment.h"

int bind_compound_arguments (object anonymous_function, object arguments);



#ifdef __cplusplus
}
#endif
#endif /*_COMPOUND_H_*/
