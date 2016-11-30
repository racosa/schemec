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

int bind_compound_arguments (object anonymous_function, object arguments){
  object variables = anonymous_function->this.compound.parameters;
  object environment = anonymous_function->this.compound.environment;
  object values = arguments;
  if(car(variables) != nil){
    if(variables->type == SFS_PAIR && car(variables)->type == SFS_PAIR && caar(variables)->type == SFS_SYMBOL){
      while (variables != nil) {
        object symbol_pair = make_pair();
        if(!strcmp(caar(variables)->this.symbol, ".") && cdr(cdr(variables)) == nil){
          symbol_pair->this.pair.car = caar(cdr(variables));
          symbol_pair->this.pair.cdr = eval_argument_list(values, environment);
          insert_symbol_in_environment(symbol_pair, environment);
          variables = cdr(cdr(variables));
          return TRUE;
        }
        else{
          symbol_pair->this.pair.car = caar(variables);
          symbol_pair->this.pair.cdr = sfs_eval(car(values), environment);
          insert_symbol_in_environment(symbol_pair, environment);
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
