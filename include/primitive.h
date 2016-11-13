/**
 * @file primitive.h
 * @author Rafael Costa Sales
 * @date Sat Nov 12 20:25:33 2016
 * @brief Primitives definition for the schemeC interpreter.
 *
 * Primitives definition for the schemeC interprete.
 */

#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environment.h"

void initialize_primitive (string symbol, primitive function);
object cons_primitive(object arguments);
object integer_add_primitive(object arguments);
object is_null_primitive(object arguments);
object char_to_integer_primitive(object arguments);

#ifdef __cplusplus
}
#endif

#endif /*_PRIMITIVE_H_*/
