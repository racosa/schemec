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

object is_null_primitive(object arguments);
object is_boolean_primitive(object arguments);
object is_symbol_primitive(object arguments);
object is_integer_primitive(object arguments);
object is_char_primitive(object arguments);
object is_string_primitive(object arguments);
object is_pair_primitive(object arguments);

object char_to_integer_primitive(object arguments);
object integer_to_char_primitive(object arguments);
object number_to_string_primitive(object arguments);
object string_to_number_primitive(object arguments);
object symbol_to_string_primitive(object arguments);
object string_to_symbol_primitive(object arguments);

object integer_addition_primitive(object arguments);
object integer_subtraction_primitive(object arguments);
object integer_multiplication_primitive(object arguments);
object integer_quotient_primitive(object arguments);
object integer_remainder_primitive(object arguments);
object integer_equal_primitive(object arguments);
object integer_less_primitive(object arguments);
object integer_greater_primitive(object arguments);

object cons_primitive(object arguments);
object car_primitive(object arguments);
object cdr_primitive(object arguments);
object set_car_primitive(object arguments);
object set_cdr_primitive(object arguments);
object list_primitive(object arguments);

object polymorphic_equality_primitive(object arguments);

#ifdef __cplusplus
}
#endif

#endif /*_PRIMITIVE_H_*/
