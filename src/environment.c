/**
 * @file environment.c
 * @author Rafael Costa
 * @date Sat Oct 16 00:56:33 2016
 * @brief Environments functions for the schemeC interpreter.
 *
 * Functions to manipulate environments in the schemeC interpreter.
 */

#include "object.h"
#include "environment.h"
#include "print.h"
#include "primitive.h"

void make_top_level_environment( void ){
  top_level_environment = make_pair();
  top_level_environment->this.pair.car = make_pair();
  top_level_environment->this.pair.car = NULL;
}

object insert_symbol_in_environment(object symbol_pair, object target_environment){

  object new_symbol_index = make_pair();
  new_symbol_index->this.pair.car = make_pair();
  new_symbol_index->this.pair.car = NULL;

  if(target_environment->this.pair.car == NULL){
    new_symbol_index->this.pair.car = symbol_pair;
    target_environment->this.pair.car = new_symbol_index;
  }
  else{
    new_symbol_index->this.pair.car = symbol_pair;
    new_symbol_index->this.pair.cdr = make_pair();
    new_symbol_index->this.pair.cdr = car(target_environment);
    target_environment->this.pair.car = new_symbol_index;
  }
  return new_symbol_index;
}

object search_symbol_in_environment(string symbol, object target_environment, int search_in_all_environments){
  object symbol_index = NULL;
  object environments = target_environment;

  if(search_in_all_environments){

    while(environments != nil){

      symbol_index = car(environments);
      if(symbol_index){
        while( symbol_index != nil ){
          if(!strcmp(car(car(symbol_index))->this.symbol, symbol)){
            return symbol_index;
          }
          symbol_index = cdr(symbol_index);
        }
      }
      environments = cdr(environments);
    }
  }
  else{
    symbol_index = car(environments);
    if(symbol_index){
      while( symbol_index != nil ){
        if(!strcmp(car(car(symbol_index))->this.symbol, symbol)){
          return symbol_index;
        }
        symbol_index = cdr(symbol_index);
      }
    }
    environments = cdr(environments);
    }
  return NULL;
  }

object make_environment (object new_environment, object target_environment){
  new_environment->this.pair.car = make_pair();
  new_environment->this.pair.car = nil;
  new_environment->this.pair.cdr = target_environment;

  if(target_environment != top_level_environment){
    target_environment = new_environment;
  }
  return new_environment;
}
