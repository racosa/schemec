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
/* -- detect in eval  */
}

object primitive_cons(object arguments){
  object pair = make_pair();
  pair->this.pair.car = car(cdr(arguments));
  pair->this.pair.cdr = car(arguments);
  return pair;

}


object primitive_add(object arguments){
  object sum = make_number(0, NUM_INTEGER);
  while (arguments != nil) {
    object operand = car(arguments);
    if (operand->type == SFS_NUMBER) {
      DEBUG_MSG("operand->this.number = %d", operand->this.number.this.integer);
      sum->this.number.this.integer += operand->this.number.this.integer;
      arguments = cdr(arguments);
    }
    else{
      return NULL;
    }
  }
  return sum;
}
