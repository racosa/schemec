/**
 * @file eval.c
 * @author racosa
 * @date Sat Dec 03 00:00:00 2016
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "object.h"
#include "environment.h"
#include "compound.h"
#include "mem.h"

object sfs_eval( object input, object target_environment ) {
  eval:
    if(input){
      /* Implementing auto-evaluation */
      if(input->type == SFS_NUMBER || input->type == SFS_CHARACTER
         || input->type == SFS_STRING || input->type == SFS_BOOLEAN || input->type == SFS_NIL){
           return input;
      }

      else{

          if(input->type == SFS_PAIR){
            /*Fixing (begin ...) input*/
            if(car(input)->type == SFS_PAIR){
              if (car(caar(input))){
                if(is_begin(car(caar(input)))){
                  DEBUG_MSG("; \" begin \" forme detected");
                  object begin_input = car(input);
                  object begin_input_arguments = cdr(begin_input);
                  input->this.pair.car = sequential_eval(begin_input_arguments, target_environment);
                }
                if(is_lambda(car(caar(input)))){
                  DEBUG_MSG("; \" lambda \" forme with arguments detected");
                  object lambda_input = car(input);
                  object lambda_arguments = cdr(input);
                  if(car(cdr(lambda_input)) != nil){
                    object new_environment = make_pair();
                    new_environment->this.pair.car = make_pair();
                    new_environment = make_environment(new_environment, target_environment);
                    object anonymous_function = make_compound(car(cdr(lambda_input)), cdr(cdr(lambda_input)), new_environment);
                    if(anonymous_function){
                      if(bind_compound_arguments(anonymous_function, lambda_arguments, anonymous_function->this.compound.environment, FALSE)){
                        return sequential_eval( anonymous_function->this.compound.body, anonymous_function->this.compound.environment );
                      }
                    }
                  }
                  WARNING_MSG("; ERROR: lambda: ill-formed expression");
                  return NULL;
                }
              }
            }

            /* Implementing lambda forme evaluation. */
            if( is_lambda(caar(input)) ){
              DEBUG_MSG("; \" lambda \" forme detected");
              object lambda_input = cdr(input);
              object anonymous_function = make_compound(car(lambda_input), cdr(cdr(lambda_input)), target_environment);
              if(anonymous_function){
                return anonymous_function;
              }
              else{
                WARNING_MSG("; ERROR: lambda: ill-formed expression");
                return NULL;
              }
            }

            /* Implementing map forme evaluation. */
            if(is_map(caar(input))){
              DEBUG_MSG("; \" map \" forme detected");
              object eval_expression = make_pair();
              object procedure = car(cdr(input));
              object list_of_lists = cdr(cdr(input));
              object list_of_lists_clone = list_of_lists;
              object list = car(list_of_lists);
              object list_clone = car(cdr(list));
              object result_list = make_pair();
              result_list = nil;
              int size_of_list = 0;
              int list_index = 0;
              int index = 0;
              /*One list case*/
              if(cdr(cdr(cdr(input))) == nil){
                list = sfs_eval(car(cdr(cdr(input))), target_environment);
                while (list != nil) {
                  eval_expression = make_pair();
                  eval_expression = nil;
                  eval_expression = cons(car(list), eval_expression);
                  eval_expression = cons(procedure, eval_expression);
                  result_list = cons(sfs_eval(eval_expression, target_environment), result_list);
                  list = cdr(list);
                  free(eval_expression);
                }
                return reverse_list(result_list);
              }
              /*More than one list case*/
              else{
                while(list_clone != nil){
                  size_of_list++;
                  list_clone = cdr(list_clone);
                }
                while(list_index < size_of_list){
                  eval_expression = nil;
                  while(list_of_lists != nil){
                    list = sfs_eval(car(list_of_lists), target_environment);
                    for(index = 0; index < list_index; index++){
                      list = cdr(list);
                    }
                    object list_element = car(list);
                    eval_expression = cons(list_element, eval_expression);
                    list_of_lists = cdr(list_of_lists);
                  }

                  eval_expression = reverse_list(eval_expression);
                  eval_expression = cons(procedure, eval_expression);
                  result_list = cons(sfs_eval(eval_expression, target_environment), result_list);

                  list_of_lists = list_of_lists_clone;
                  free(eval_expression);
                  list_index++;
                }
                return reverse_list(result_list);
              }

            }

            /* Implementing let forme evaluation. */
            if( is_let(caar(input)) ){
              DEBUG_MSG("; \" let \" forme detected");
              object bindings = car(cdr(input));
              object body = cdr(cdr(input));
              if(bindings->type != SFS_PAIR || (car(body)->type != SFS_PAIR && car(body)->type != SFS_NIL)){
                WARNING_MSG("; ERROR: let: ill-formed expression");
                return NULL;
              }
              else{
                object extended_environment = make_pair();
                extended_environment = make_environment(extended_environment, target_environment);
                /* Multiple bindings case */
                if(bindings->type == SFS_PAIR && car(bindings)->type == SFS_PAIR && caar(bindings)->type == SFS_PAIR ){
                  while(bindings != nil){
                    object variable = cons(caar(bindings), nil);
                    object anonymous_function = make_compound(variable, body, extended_environment);
                    if(anonymous_function){
                      if(cdr(cdr(car(bindings))) != nil){
                        WARNING_MSG("; ERROR: invalid init in let forme expression");
                        return NULL;
                      }
                      else{
                        object init = car(cdr(car(bindings)));
                        init = sfs_eval(init, target_environment);
                        init = cons( init , nil);
                        bind_compound_arguments(anonymous_function, init, extended_environment, FALSE);
                      }
                    }
                    else{
                      WARNING_MSG("; ERROR: invalid variables in let forme expression");
                      return NULL;
                    }
                    bindings = cdr(bindings);
                  }
                }
                /* Single binding case */
                else if(bindings->type == SFS_PAIR && car(bindings)->type == SFS_PAIR && caar(bindings)->type == SFS_SYMBOL ){
                  object variable = cons(car(bindings), nil);
                  object anonymous_function = make_compound(variable, body, extended_environment);
                  if(anonymous_function){
                    if(cdr(cdr(bindings)) != nil){
                      WARNING_MSG("; ERROR: invalid init in let forme expression");
                      return NULL;
                    }
                    else{
                      object init = car(cdr(bindings));
                      init = sfs_eval(init, target_environment);
                      init = cons(init , nil);
                      bind_compound_arguments(anonymous_function, init, extended_environment, FALSE);
                    }
                  }
                  else{
                    WARNING_MSG("; ERROR: invalid variables in let forme expression");
                    return NULL;
                  }

                }
                return sequential_eval(body, extended_environment);
              }
            }

            /* Implementing let* forme evaluation. */
            if( is_let_asterisc(caar(input)) ){
              DEBUG_MSG("; \" let* \" forme detected");
              object bindings = car(cdr(input));
              object body = cdr(cdr(input));

              if(bindings->type != SFS_PAIR || (car(body)->type != SFS_PAIR && car(body)->type != SFS_NIL)){
                WARNING_MSG("; ERROR: let*: ill-formed expression");
                return NULL;

              }
              else{
                object extended_environment = make_pair();
                extended_environment = make_environment(extended_environment, target_environment);
                /* Multiple bindings case */
                if(bindings->type == SFS_PAIR && car(bindings)->type == SFS_PAIR && caar(bindings)->type == SFS_PAIR ){
                  while(bindings != nil){
                    object variable = cons(caar(bindings), nil);
                    object anonymous_function = make_compound(variable, body, extended_environment);
                    if(anonymous_function){
                      if(cdr(cdr(car(bindings))) != nil){
                        WARNING_MSG("; ERROR: invalid init in let* forme expression");
                        return NULL;
                      }
                      else{
                        object init = car(cdr(car(bindings)));
                        init = sfs_eval(init, target_environment);
                        init = cons( init , nil);
                        bind_compound_arguments(anonymous_function, init, extended_environment, FALSE);
                        target_environment = extended_environment;
                      }
                    }
                    else{
                      WARNING_MSG("; ERROR: invalid variables in let* forme expression");
                      return NULL;
                    }
                    bindings = cdr(bindings);
                  }
                }
                /* Single binding case */
                else if(bindings->type == SFS_PAIR && car(bindings)->type == SFS_PAIR && caar(bindings)->type == SFS_SYMBOL ){
                  object variable = cons(car(bindings), nil);
                  object anonymous_function = make_compound(variable, body, extended_environment);
                  if(anonymous_function){
                    if(cdr(cdr(bindings)) != nil){
                      WARNING_MSG("; ERROR: invalid init in let* forme expression");
                      return NULL;
                    }
                    else{
                      object init = car(cdr(bindings));
                      init = sfs_eval(init, target_environment);
                      init = cons(init , nil);
                      bind_compound_arguments(anonymous_function, init, extended_environment, FALSE);
                      target_environment = extended_environment;
                    }
                  }
                  else{
                    WARNING_MSG("; ERROR: invalid variables in let* forme expression");
                    return NULL;
                  }
                }
                return sequential_eval(body, extended_environment);
              }
            }

            /* Implementing begin forme evaluation. */
            if( is_begin(caar(input)) ){
              DEBUG_MSG("; \" begin \" forme detected");
              object begin_arguments = cdr(input);

              return sequential_eval(begin_arguments, target_environment);
            }

            /* Implementing quote forme evaluation. */
            if( is_quote(caar(input)) ){
              if( cdr(cdr(input)) == nil ){
                DEBUG_MSG("; \" quote \" forme detected");
                return car(cdr(input));
              }
              else{
                WARNING_MSG("; ERROR: quote: missing or extra arguments");
                return NULL;
              }
            }

            /* Implementing define forme evaluation. */
            else if(is_define(caar(input))){
              object function = make_pair();
              function->this.pair.car = NULL;
              object new_environment = make_pair();
              object parameters = NULL;
              object body = NULL;
              object function_symbol = NULL;

              if(is_lambda(car(caar(cdr(cdr(input)))))){
                DEBUG_MSG("; \" define function\" forme detected");
                parameters = car(cdr(car(cdr(cdr(input)))));
                body = cdr(cdr(car(cdr(cdr(input)))));
                function_symbol = car(car(cdr(input)));
                function = define_function(function, function_symbol, parameters, body, target_environment, target_environment);
                if(function){
                  DEBUG_MSG("; \" define function\" returning compound");
                  return car(car(function));
                }
                WARNING_MSG("; ERROR: define: ill-formed expression");
                return NULL;
              }

            else if(is_lambda(caar(caar(cdr(cdr(input)))))){
              DEBUG_MSG("; \" define function\" with binded variables forme detected");
              object new_binded_environment = make_pair();
              new_binded_environment = make_environment(new_binded_environment, target_environment);
              object lambda_input = caar(cdr(cdr(input)));
              object lambda_arguments = cdr(car(cdr(cdr(input))));
              object binded_parameters = car(cdr(lambda_input));

              object anonymous_dummy_function = make_compound(binded_parameters, nil, new_binded_environment);

              bind_compound_arguments(anonymous_dummy_function, lambda_arguments,
                                      anonymous_dummy_function->this.compound.environment, FALSE);

              target_environment = new_binded_environment;

              new_environment = make_environment(new_environment, target_environment);
              parameters = car(cdr(car(cdr(cdr(lambda_input)))));
              body = cdr(cdr(car(cdr(cdr(lambda_input)))));

              function_symbol = car(car(cdr(input)));
              function = define_function(function, function_symbol, parameters, body, target_environment, new_environment);

              if(function){
                  DEBUG_MSG("; \" define function\" returning compound");
                  return car(car(function));
              }
              WARNING_MSG("; ERROR: define: ill-formed expression");
              return NULL;
            }

            else if(car(cdr(input))->type == SFS_PAIR && caar(cdr(input))->type == SFS_PAIR){
              DEBUG_MSG("; Implicit \" define function\" forme detected");
              parameters = cdr(car(cdr(input)));
              body = cdr(cdr(input));
              function_symbol = caar(car(cdr(input)));
              new_environment = make_environment(new_environment, target_environment);

              function = define_function(function, function_symbol, parameters, body, target_environment, target_environment);
              if(function){
                DEBUG_MSG("; \" define function\" returning compound");
                return car(car(function));
              }
              WARNING_MSG("; ERROR: define: ill-formed expression");
              return NULL;
            }

              else if(cdr(cdr(cdr(input))) == nil){

                DEBUG_MSG("; \" define \" forme detected");
                object symbol = make_pair();
                object symbol_value = sfs_malloc(sizeof(*symbol_value));
                symbol->this.pair.car = NULL;

                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol, target_environment, FALSE );
                symbol_value = sfs_eval(car(cdr(cdr(input))), target_environment);
                if(symbol_value){
                    symbol->this.pair.car->this.pair.cdr = symbol_value;
                    return car(car(symbol));
                  }
                else{
                    return NULL;
                }
              }
              else {
                WARNING_MSG("; ERROR: define: missing or extra arguments");
                return NULL;
              }
            }

            /* Implementing set! forme evaluation. */
            else if(is_set(caar(input))){
              if(cdr(cdr(cdr(input))) == nil){
                DEBUG_MSG("; \" set! \" forme detected");
                object symbol = make_pair();
                symbol->this.pair.car = NULL;
                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol, target_environment, TRUE );

                if(cdr(car(symbol)) != NULL){
                  object old_symbol_value = make_object(cdr(car(symbol))->type);
                  old_symbol_value = cdr(car(symbol));
                  symbol->this.pair.car->this.pair.cdr = sfs_eval(car(cdr(cdr(input))), target_environment);
                  return old_symbol_value;
                }
                else{
                  WARNING_MSG("; ERROR: unbound variable");
                  return NULL;
                }
              }
              else {
                WARNING_MSG("; ERROR: set!: missing or extra arguments");
                return NULL;
              }

            }

            /* Implementing if forme evaluation. */
            else if(is_if(caar(input))){

              if(cdr(cdr(cdr(cdr(input)))) == nil || cdr(cdr(cdr(input)))){

                DEBUG_MSG("; \" if \" forme detected");
                object predicate = sfs_eval(car(cdr(input)), target_environment);
                if(predicate){
                  if(predicate != false){
                    DEBUG_MSG("; (predicate) is (true), evaluating (consequence)");
                    /*input = car(cdr(cdr(input)));*/
                    /*enabling multiple line body in if statements*/
                    input = cons(car(cdr(cdr(input))), nil);
                    return sequential_eval(input, target_environment);
                  }
                  else{
                    DEBUG_MSG("; (predicate) is (false), evaluating (alternative)");
                    /*input = car(cdr(cdr(cdr(input))));*/
                    /*enabling multiple line body in if statements*/
                    return sequential_eval(cdr(cdr(cdr(input))) , target_environment);
                  }
                  /*Goto begin of eval function*/
                  goto eval;
                }
                else{
                /*  WARNING_MSG("; ERROR: if: object in predicate is not applicable"); */
                  return NULL;
                }
              }
              else {
                WARNING_MSG("; ERROR: if: missing or extra arguments");
                return NULL;
              }
            }

            /* Implementing and forme evaluation. */
            else if(is_and(caar(input))){
              DEBUG_MSG("; \" and \" forme detected");
              input = cdr(input);
              while(cdr(input) != nil){
                if(sfs_eval(car(input), target_environment) == false){
                  return false;
                }
                input = cdr(input);

                if(input == NULL) { /* and without arguments */
                  return true;
                }
              }
                return sfs_eval(car(input), target_environment);
              }

            /* Implementing or forme evaluation. */
            else if(is_or(caar(input))){
              DEBUG_MSG("; \" or \" forme detected");
              input = cdr(input);
              if(car(input) == NULL){ /* or without arguments */
                return false;
              }
              while(cdr(input) != nil){
                if(sfs_eval(car(input), target_environment) != false){
                  return sfs_eval(car(input), target_environment);
                }
                input = cdr(input);
              }
              return sfs_eval(car(input), target_environment);
            }

            /* Implementing primitive evaluation. */
            else if(is_primitive(car(input))){
              DEBUG_MSG("; primitive \" %s \" detected", caar(input)->this.symbol);
              primitive primitive_function = cdr(car(input))->this.primitive.function;
              object arguments = nil;
              object reverse_arguments = nil;
              while(cdr(input) != nil){
                object evaluated_argument = sfs_eval(car(cdr(input)), target_environment);
                if(evaluated_argument){
                  arguments = cons(evaluated_argument, arguments);
                  input = cdr(input);
                }
                else{
                  return NULL;
                }
              }
              while(arguments != nil){
                reverse_arguments = cons(car(arguments), reverse_arguments);
                arguments = cdr(arguments);
              }
              return primitive_function(reverse_arguments);
            }

            else if(is_compound(car(input))){

              object function = cdr(car(input));
              object arguments = cdr(input);
              /* Fixing case for binded environment in initialization*/
              if(function->this.compound.environment != top_level_environment){
                target_environment = function->this.compound.environment;
              }
              object extended_environment = make_pair();
              extended_environment = make_environment(extended_environment, target_environment);

              if(bind_compound_arguments(function, arguments, extended_environment, FALSE)){
                DEBUG_MSG("; Extended environment updated successfully..");
/*
                printf("\n---------------------- printing updated environment\n");
                sfs_print(new_environment);
                printf("\n---------------------- printing updated environment\n");
*/
                return sequential_eval( function->this.compound.body, extended_environment );
              }
              else{
                DEBUG_MSG(";ERROR: failed to update environment");
                return NULL;
              }

            }

            /* Implementing symbol evaluation. */
            else if(car(input)->type == SFS_SYMBOL){

              if(is_primitive(input)){
                 return input;
              }
              else{
                object symbol;
                symbol = search_symbol_in_environment( car(input)->this.symbol, target_environment, TRUE );
                if(cdr(car(symbol)) != NULL ){
                  return cdr(car(symbol));
                }
                WARNING_MSG("; ERROR: unbound variable: %s", car(car(symbol))->this.symbol );
                return NULL;
              }
            }

            else{
                WARNING_MSG("; ERROR: wrong type to apply");
                return NULL;
              }
            }
            else{
              WARNING_MSG("; ERROR: wrong type to apply");
              return NULL;
            }
        }
    }
    else{
      DEBUG_MSG("; ERROR: evaluation exception");
      return NULL;
    }
}

object sequential_eval(object arguments, object target_environment){
  object result = NULL;
  while(arguments != nil){
    result = sfs_eval(car(arguments), target_environment);
    if(result){
      arguments = cdr(arguments);
    }
    else{
      return NULL;
    }
  }
  return result;
}
