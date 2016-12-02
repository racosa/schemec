
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
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
  /*  printf("\nPrinting target_environment---------------------------------\n");
    sfs_print( target_environment );
    printf("\n--------------------------------------------------------------\n");
  */
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
                    /*object new_environment1 = make_pair();*/
                    new_environment = make_environment(new_environment, target_environment);

                    object anonymous_function = make_compound(car(cdr(lambda_input)), cdr(cdr(lambda_input)), new_environment);
                    if(anonymous_function){
                      if(bind_compound_arguments(anonymous_function, lambda_arguments)){
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
              object new_environment = make_pair();
              new_environment = make_environment(new_environment, target_environment);
              object lambda_input = cdr(input);
              object anonymous_function = make_compound(car(lambda_input), cdr(cdr(lambda_input)), new_environment);
              if(anonymous_function){
/*
                sfs_print(target_environment);
  */              return sequential_eval( anonymous_function->this.compound.body, anonymous_function->this.compound.environment );

            /*   return anonymous_function; */
              }
              else{
                WARNING_MSG("; ERROR: lambda: ill-formed expression");
                return NULL;
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
                new_environment = make_environment(new_environment, target_environment);
                function = define_function(function, function_symbol, parameters, body, target_environment, new_environment);
                if(function){
                  DEBUG_MSG("; \" define function\" returning compound");
                  return car(car(function));
                }
                DEBUG_MSG("; ERROR: define: ill-formed expression");
                return NULL;
              }

            else if(is_lambda(caar(caar(cdr(cdr(input)))))){
              DEBUG_MSG("; \" define function\" with binded variables forme detected");
              new_environment = make_environment(new_environment, target_environment);

              object lambda_input = caar(cdr(cdr(input)));
              object lambda_arguments = cdr(car(cdr(cdr(input))));
/*
              printf("----------------- arguments\n");
              sfs_print(lambda_arguments);
              printf("-----------------  \n");
*/
              parameters = car(cdr(lambda_input));
/*
              printf("----------------- parameters\n");
              sfs_print(parameters);
              printf("-----------------  \n");
*/
              body = cdr(cdr(lambda_input));
/*
              printf("----------------- body\n");
              sfs_print(body);
              printf("-----------------  \n");
*/

              function_symbol = car(car(cdr(input)));
              function = define_function(function, function_symbol, parameters, body, target_environment, new_environment);
              if(function){
                if(bind_compound_arguments(function->this.pair.car->this.pair.cdr, lambda_arguments)){
/*
                  printf("----------------- compound envi\n");
                  sfs_print(function->this.pair.car->this.pair.cdr->this.compound.environment);
                  printf("-----------------  \n");
*/
                  DEBUG_MSG("; \" define function\" returning compound");
                  return car(car(function));

                }

              }
              DEBUG_MSG("; ERROR: define: ill-formed expression");
              return NULL;



            }

            else if(car(cdr(input))->type == SFS_PAIR && caar(cdr(input))->type == SFS_PAIR){
              DEBUG_MSG("; Implicit \" define function\" forme detected");
              parameters = cdr(car(cdr(input)));
              body = cdr(cdr(input));
              function_symbol = caar(car(cdr(input)));
              new_environment = make_environment(new_environment, target_environment);

              function = define_function(function, function_symbol, parameters, body, target_environment, new_environment);
              if(function){
                DEBUG_MSG("; \" define function\" returning compound");
                return car(car(function));
              }
              DEBUG_MSG("; ERROR: define: ill-formed expression");
              return NULL;
            }

              else if(cdr(cdr(cdr(input))) == nil){

                DEBUG_MSG("; \" define \" forme detected");
                object symbol = make_pair();
                object symbol_value = sfs_malloc(sizeof(*symbol_value));
                symbol->this.pair.car = NULL;

                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol, target_environment );
                if(cdr(car(symbol)) != nil ){
                  symbol_value = sfs_eval(car(cdr(cdr(input))), target_environment);
                  if(symbol_value) {
                    symbol->this.pair.car->this.pair.cdr = symbol_value;
                    return car(car(symbol));
                  }
                  else{
                    return NULL;
                  }

                 }
                 else{
                  symbol_value = sfs_eval(car(cdr(cdr(input))), target_environment);
                  if(symbol_value){
                    symbol->this.pair.car->this.pair.cdr = symbol_value;
                    return car(car(symbol));
                  }
                  else{
                    return NULL;
                  }

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
                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol, target_environment );

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
                    input = car(cdr(cdr(input)));
                  }
                  else{
                    DEBUG_MSG("; (predicate) is (false), evaluating (alternative)");
                    input = car(cdr(cdr(cdr(input))));

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
              DEBUG_MSG("; compound \" %s \" detected", caar(input)->this.symbol);

              object function = cdr(car(input));
              object arguments = cdr(input);
/*
              sfs_print(function->this.compound.body);

*/
              if(bind_compound_arguments(function, arguments)){
                DEBUG_MSG("; Compound binded ..");
                return sequential_eval( function->this.compound.body, function->this.compound.environment );
              }
              else{
                DEBUG_MSG(";ERROR: binding compound");
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
                if(target_environment == top_level_environment){
                  DEBUG_MSG("Searching in top_level_environment..");
                }
              /*  if(target_environment == current_environment){
                  DEBUG_MSG("Searching in current_environment..");
                }*/

                symbol = search_symbol_in_environment( car(input)->this.symbol, target_environment );
                if(cdr(car(symbol)) != NULL ){
                  /*DEBUG_MSG("; Variable found in top level environment");*/
                  return cdr(car(symbol));
                }
                WARNING_MSG("; ERROR: unbound variable: %s", car(car(symbol))->this.symbol );
                return NULL;
              }
            }

            else{
              /*
                return input;
                */
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
      DEBUG_MSG("exception returning NULL");
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

int is_lambda( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "lambda" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_begin( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "begin" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_compound( object object ){
  if(cdr(object)){
    if(cdr(object)->type == SFS_COMPOUND){
      return TRUE;
    }
  }
  return FALSE;
}

int is_primitive( object object ){
  if(cdr(object)){
    if(cdr(object)->type == SFS_PRIMITIVE){
      return TRUE;
    }
  }
  return FALSE;
}

int is_quote( object object ){
  if(object){
    if ( !strcmp( object->this.symbol, "quote" ) || !strcmp( object->this.symbol, "\'" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_define( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "define" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_set( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "set!" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_if( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "if" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_and( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "and" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_or( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "or" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_forme(object symbol){
    if(is_quote(symbol) || is_define(symbol) || is_set(symbol)
       || is_if(symbol) || is_and(symbol) || is_or(symbol) || is_begin(symbol)){
      return TRUE;
    }
    return FALSE;
}
