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

/* Integer arithmetic primitives */
object integer_add_primitive(object arguments){
  object sum = make_number(0, NUM_INTEGER);
  while (arguments != nil) {
    object operand = car(arguments);
    if (operand->type == SFS_NUMBER) {
      sum->this.number.this.integer += operand->this.number.this.integer;
      arguments = cdr(arguments);
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer-add is not of the correct type");
      return NULL;
    }
  }
  return sum;
}

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

/* Polymorphic equality
object polymorphic_equality_primitive(object arguments){} */
