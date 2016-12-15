/**
 * @file compound.c
 * @author Rafael Costa Sales
 * @date Sun Nov 27 21:06:43 2016
 * @brief Compound functions for the schemeC interpreter.
 *
 * Compound functions for the schemeC interpreter.
 */

#include "compound.h"
#include "eval.h"
#include "print.h"


object define_function(object function, object function_symbol, object parameters, object body,
  object target_environment, object new_environment){
  function = search_symbol_in_environment (function_symbol->this.symbol, target_environment, TRUE);
  if(function){
    object compound = make_compound(parameters, body, new_environment);
    if (compound){
      function->this.pair.car->this.pair.cdr = compound;
      return function;
    }
  }
  return NULL;
}


int bind_compound_arguments (object anonymous_function, object arguments, object bind_environment, int search_in_all_environments){

  object variables = anonymous_function->this.compound.parameters;
  object environment = bind_environment;
  object values = arguments;

  if(car(variables) != nil){

    if(variables->type == SFS_PAIR && car(variables)->type == SFS_PAIR && caar(variables)->type == SFS_SYMBOL){
      while (variables != nil) {
        object symbol_pair = make_pair();
        if(!strcmp(caar(variables)->this.symbol, ".") && cdr(cdr(variables)) == nil){
          object symbol_index = search_symbol_in_environment(caar(cdr(variables))->this.symbol, environment, search_in_all_environments);
          if(symbol_index){
            symbol_index->this.pair.car->this.pair.cdr = eval_argument_list(values, environment);
          }
          else{
            symbol_pair->this.pair.car = caar(cdr(variables));
            symbol_pair->this.pair.cdr = eval_argument_list(values, environment);
            insert_symbol_in_environment(symbol_pair, environment);
          }
          variables = cdr(cdr(variables));
          return TRUE;
        }
        else{
          object symbol_index = search_symbol_in_environment(caar(variables)->this.symbol, environment, search_in_all_environments);
          if(symbol_index){
            symbol_index->this.pair.car->this.pair.cdr = sfs_eval(car(values), environment);
          }
          else{
            symbol_pair->this.pair.car = caar(variables);
            symbol_pair->this.pair.cdr = sfs_eval(car(values), environment);
            insert_symbol_in_environment(symbol_pair, environment);
          }
          variables = cdr(variables);
          values = cdr(values);
        }
      }
      if(cdr(values)){
        return FALSE;
      }
    }

    else if(variables->type == SFS_PAIR && car(variables)->type == SFS_SYMBOL){
      object symbol_index = search_symbol_in_environment(car(variables)->this.symbol, environment, search_in_all_environments);
      if(symbol_index){
        symbol_index->this.pair.car->this.pair.cdr = eval_argument_list(values, environment);
      }
      else{
        object symbol_pair = make_pair();
        symbol_pair->this.pair.car = car(variables);
        symbol_pair->this.pair.cdr = eval_argument_list(values, environment);
        insert_symbol_in_environment(symbol_pair, environment);
      }
    }
    return TRUE;
  }
  return FALSE;
}

object eval_argument_list(object values, object environment){
  object evaluated_values_list = nil;
  object reverse_evaluated_values_list = nil;
  while(values != nil){
    object evaluated_value = sfs_eval(car(values), environment);
    if(evaluated_value){
       evaluated_values_list = cons(evaluated_value,  evaluated_values_list);
       values = cdr(values);
    }
  }
  while(evaluated_values_list != nil){
    reverse_evaluated_values_list = cons(car(evaluated_values_list), reverse_evaluated_values_list);
    evaluated_values_list = cdr(evaluated_values_list);
  }
  return reverse_evaluated_values_list;
}
