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
  function = search_symbol_in_environment (function_symbol->this.symbol, target_environment);
  if(function){
    function->this.pair.car->this.pair.cdr = make_compound(parameters, body, new_environment);
    return function;
  }
  return NULL;
}


int bind_compound_arguments (object anonymous_function, object arguments){

  /*
  if(car(anonymous_function->this.compound.parameters) != nil){

    if(anonymous_function->this.compound.parameters->type == SFS_PAIR && car(anonymous_function->this.compound.parameters)->type == SFS_PAIR && caar(anonymous_function->this.compound.parameters)->type == SFS_SYMBOL){
      while (anonymous_function->this.compound.parameters != nil) {
        object symbol_pair = make_pair();
        if(!strcmp(caar(anonymous_function->this.compound.parameters)->this.symbol, ".") && cdr(cdr(anonymous_function->this.compound.parameters)) == nil){

          object symbol_index = search_symbol_in_environment(caar(cdr(anonymous_function->this.compound.parameters))->this.symbol, anonymous_function->this.compound.environment);
          if(symbol_index){

            symbol_index->this.pair.car->this.pair.cdr = eval_argument_list(arguments, anonymous_function->this.compound.environment);
          }
          else{
            symbol_pair->this.pair.car = caar(cdr(anonymous_function->this.compound.parameters));
            symbol_pair->this.pair.cdr = eval_argument_list(arguments, anonymous_function->this.compound.environment);
            insert_symbol_in_environment(symbol_pair, anonymous_function->this.compound.environment);
          }
          anonymous_function->this.compound.parameters = cdr(cdr(anonymous_function->this.compound.parameters));
          return TRUE;
        }
        else{
          object symbol_index = search_symbol_in_environment(caar(anonymous_function->this.compound.parameters)->this.symbol, anonymous_function->this.compound.environment);
          if(symbol_index){
            symbol_index->this.pair.car->this.pair.cdr = sfs_eval(car(arguments), anonymous_function->this.compound.environment);
          }
          else{
            symbol_pair->this.pair.car = caar(anonymous_function->this.compound.parameters);
            symbol_pair->this.pair.cdr = sfs_eval(car(arguments), anonymous_function->this.compound.environment);
            insert_symbol_in_environment(symbol_pair, anonymous_function->this.compound.environment);
          }
          anonymous_function->this.compound.parameters = cdr(anonymous_function->this.compound.parameters);
          arguments = cdr(arguments);
        }
      }
      if(cdr(arguments)){
        return FALSE;
      }
    }

    else if(anonymous_function->this.compound.parameters->type == SFS_PAIR && car(anonymous_function->this.compound.parameters)->type == SFS_SYMBOL){
      object symbol_pair = make_pair();
      symbol_pair->this.pair.car = car(anonymous_function->this.compound.parameters);

      symbol_pair->this.pair.cdr = eval_argument_list(arguments, anonymous_function->this.compound.environment);
      insert_symbol_in_environment(symbol_pair, anonymous_function->this.compound.environment);
    }
    return TRUE;
  }
  return FALSE;


*/


  object variables = anonymous_function->this.compound.parameters;
  object environment = anonymous_function->this.compound.environment;
  object values = arguments;

  if(car(variables) != nil){

    if(variables->type == SFS_PAIR && car(variables)->type == SFS_PAIR && caar(variables)->type == SFS_SYMBOL){
      while (variables != nil) {
        object symbol_pair = make_pair();
        if(!strcmp(caar(variables)->this.symbol, ".") && cdr(cdr(variables)) == nil){
          /*
          object symbol_index = search_symbol_in_environment(caar(cdr(variables))->this.symbol, environment);
          if(symbol_index){

            symbol_index->this.pair.car->this.pair.cdr = eval_argument_list(values, environment);
          }
          else{*/
            symbol_pair->this.pair.car = caar(cdr(variables));
            symbol_pair->this.pair.cdr = eval_argument_list(values, environment);
            insert_symbol_in_environment(symbol_pair, environment);
          /*}*/
/*
          printf("\n---------------- binding environment\n");
          sfs_print(environment);
          printf("\n---------------- environment\n" );
*/
          variables = cdr(cdr(variables));
          return TRUE;
        }
        else{
          /*
          object symbol_index = search_symbol_in_environment(caar(variables)->this.symbol, environment);
          if(symbol_index){
            symbol_index->this.pair.car->this.pair.cdr = sfs_eval(car(values), environment);
          }
          else{
          */
            symbol_pair->this.pair.car = caar(variables);
            symbol_pair->this.pair.cdr = sfs_eval(car(values), environment);
            insert_symbol_in_environment(symbol_pair, environment);
          /*}*/
/*
          printf("\n---------------- binding environment\n");
          sfs_print(environment);
          printf("\n---------------- environment\n" );
*/
          variables = cdr(variables);
          values = cdr(values);
        }
      }
      if(cdr(values)){
        return FALSE;
      }
    }

    else if(variables->type == SFS_PAIR && car(variables)->type == SFS_SYMBOL){
      object symbol_pair = make_pair();
      symbol_pair->this.pair.car = car(variables);

      symbol_pair->this.pair.cdr = eval_argument_list(values, environment);
      insert_symbol_in_environment(symbol_pair, environment);
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
