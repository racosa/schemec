/**
 * @file environment.c
 * @author Rafael Costa Sales
 * @date Sat Oct 16 00:56:33 2016
 * @brief Environments stuff for the schemeC interpreter.
 *
 * Functions to manipulate environments in the schemeC interpreter.
 */

#include "object.h"
#include "environment.h"

object make_top_level_environment( void ){
  environment = make_pair();
  environment->this.pair.car = nil;
  return environment;
}

object insert_symbol_in_environment(object environment, object symbol_pair){
  object new_symbol_index = make_pair();
  if(environment->this.pair.car == NULL){
    environment->this.pair.car = new_symbol_index;
    environment->this.pair.car->this.pair.car = symbol_pair;
    environment->this.pair.car->this.pair.cdr = nil;
  }
  else{
    new_symbol_index->this.pair.car = symbol_pair;
    new_symbol_index->this.pair.cdr = car(environment);
    environment->this.pair.car = new_symbol_index;
  }
  return new_symbol_index;
}

object search_symbol_in_environment(object environment, string symbol){
  object symbol_index = car(environment);
  while(symbol_index != nil){
    if(!strcmp(car(car(symbol_index))->this.symbol, symbol)){
      return symbol_index;
    }
    symbol_index = cdr(symbol_index);
  }
  return 0;
}