/**
 * @file primitive.c
 * @author Rafael Costa Sales
 * @date Sat Nov 12 20:25:33 2016
 * @brief Primitive functions for the schemeC interpreter.
 *
 * Definition of the primitive functions for the schemeC interpreter.
 */

 #include "primitive.h"

void initialize_primitive (string symbol, primitive function){
  object symbol_pair = make_symbol(symbol);
  symbol_pair->this.pair.cdr = make_primitive(function);
  insert_symbol_in_environment(symbol_pair);
}

/* Predicate primitives */
object is_null_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments) == nil){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure null? ");
  return NULL;
}

object is_boolean_primitive(object arguments);
object is_symbol_primitive(object arguments);
object is_integer_primitive(object arguments);
object is_char_primitive(object arguments);
object is_string_primitive(object arguments);
object is_pair_primitive(object arguments);

/* Type convertion primitives */
object char_to_integer_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_CHARACTER){
      object integer = make_number(car(arguments)->this.character, NUM_INTEGER);
      return integer;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure char->integer is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure char->integer ");
  return NULL;
}

object integer_to_char_primitive(object arguments);
object number_to_string_primitive(object arguments);
object string_to_number_primitive(object arguments);
object symbol_to_string_primitive(object arguments);
object string_to_symbol_primitive(object arguments);

/* Integer arithmetic primitives */
object integer_addition_primitive(object arguments){
  object result = make_number(0, NUM_INTEGER);
  while (arguments != nil) {
    object operand = car(arguments);
    if (operand->type == SFS_NUMBER) {
      result->this.number.this.integer += operand->this.number.this.integer;
      arguments = cdr(arguments);
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer-add is not of the correct type");
      return NULL;
    }
  }
  return result;
}

object integer_subtraction_primitive(object arguments);
object integer_multiplication_primitive(object arguments);
object integer_quotient_primitive(object arguments);
object integer_remainder_primitive(object arguments);
object integer_equal_primitive(object arguments);
object integer_less_primitive(object arguments);
object integer_greater_primitive(object arguments);

/* List manipulation primitives */
object cons_primitive(object arguments){
  if(cdr(cdr(arguments)) == nil){
    object pair = make_pair();
    pair->this.pair.car = car(cdr(arguments));
    pair->this.pair.cdr = car(arguments);
    return pair;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure cons ");
  return NULL;
}

object car_primitive(object arguments){
  if(caar(arguments)){
    return caar(arguments);
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure car ");
  return NULL;
}

object cdr_primitive(object arguments){
  if(caar(arguments)){
    return cdr(car(arguments));
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure cdr ");
  return NULL;
}

object set_car_primitive(object arguments);
object set_cdr_primitive(object arguments);
object list_primitive(object arguments);

/* Polymorphic equality */
object polymorphic_equality_primitive(object arguments);
