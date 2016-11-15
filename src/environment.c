/**
 * @file environment.c
 * @author Rafael Costa Sales
 * @date Sat Oct 16 00:56:33 2016
 * @brief Environments functions for the schemeC interpreter.
 *
 * Functions to manipulate environments in the schemeC interpreter.
 */

#include "object.h"
#include "environment.h"
#include "print.h"
#include "primitive.h"

object make_top_level_environment( void ){
  environment = make_pair();
  environment->this.pair.car = make_pair();
  environment->this.pair.car = NULL;
  return environment;
}

object insert_symbol_in_environment(object symbol_pair){
  object new_symbol_index = make_pair();
  new_symbol_index->this.pair.car = make_pair();
  new_symbol_index->this.pair.car = NULL;

  if(environment->this.pair.car == NULL){
    new_symbol_index->this.pair.car = symbol_pair;
    environment->this.pair.car = new_symbol_index;
  }
  else{
    new_symbol_index->this.pair.car = symbol_pair;
    new_symbol_index->this.pair.cdr = make_pair();
    new_symbol_index->this.pair.cdr = car(environment);
    environment->this.pair.car = new_symbol_index;
  }
  return new_symbol_index;
}

object search_symbol_in_environment(string symbol){

  object symbol_index = car(environment);

  while( symbol_index != nil ){
    if(!strcmp(car(car(symbol_index))->this.symbol, symbol)){
      return symbol_index;
    }
    symbol_index = cdr(symbol_index);
  }
  return NULL;
}

void initialize_formes(void){
  DEBUG_MSG("; Initializing formes in the top level environment..");
  int i;
  object symbol_name = make_object(SFS_SYMBOL);
  object symbol_pair = make_pair();
  symbol_pair->this.pair.car = make_object(SFS_SYMBOL);

  const char *formes[] = {
    "quote",
    "define",
    "set!",
    "if",
    "and",
    "or"
  };
  for(i=0 ; i < 6 ; i++){
    strcpy( symbol_name->this.symbol, formes[i] );
    symbol_pair->this.pair.car = symbol_name;
    insert_symbol_in_environment(symbol_pair);
  }
  DEBUG_MSG("; ---------------------------------------- Completed");
}

void initialize_primitives(void){
  DEBUG_MSG("; Initializing primitives in the top level environment..");

  initialize_primitive("null?", is_null_primitive);
  initialize_primitive("boolean?", is_boolean_primitive);
  initialize_primitive("symbol?", is_symbol_primitive);
  initialize_primitive("integer?", is_integer_primitive);
  initialize_primitive("char?", is_char_primitive);
  initialize_primitive("string?", is_string_primitive);
  initialize_primitive("pair?", is_pair_primitive);

  initialize_primitive("char->integer", char_to_integer_primitive);
  initialize_primitive("integer->char", integer_to_char_primitive);
  initialize_primitive("number->string", number_to_string_primitive);
  initialize_primitive("string->number", string_to_number_primitive);
  initialize_primitive("symbol->string", symbol_to_string_primitive);
  initialize_primitive("string->symbol", string_to_symbol_primitive);

  initialize_primitive("+", integer_addition_primitive);
  initialize_primitive("-", integer_subtraction_primitive);
  initialize_primitive("*", integer_multiplication_primitive);
  initialize_primitive("quotient", integer_quotient_primitive);
  initialize_primitive("remainder", integer_remainder_primitive);
  initialize_primitive("=", integer_equal_primitive);
  initialize_primitive("<", integer_less_primitive);
  initialize_primitive(">", integer_greater_primitive);

  initialize_primitive("cons", cons_primitive);
  initialize_primitive("car", car_primitive);
  initialize_primitive("cdr", cdr_primitive);
  initialize_primitive("list", list_primitive);
  initialize_primitive("set-car!", set_car_primitive);
  initialize_primitive("set-cdr!", set_cdr_primitive);

  initialize_primitive("eq?", polymorphic_equality_primitive);

  DEBUG_MSG("; ------------------------------------------- Completed");
}
