
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
#include "mem.h"

object sfs_eval( object input ) {
    eval:
    if(input){
      /* Implementing auto-evaluation */
      if(input->type == SFS_NUMBER || input->type == SFS_CHARACTER
         || input->type == SFS_STRING || input->type == SFS_BOOLEAN || input->type == SFS_NIL){
           return input;
         }

      else{

          if(input->type == SFS_PAIR){

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
              if(cdr(cdr(cdr(input))) == nil){

                DEBUG_MSG("; \" define \" forme detected");
                object symbol = make_pair();
                object symbol_value = sfs_malloc(sizeof(*symbol_value));
                symbol->this.pair.car = NULL;

                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );
                if(cdr(car(symbol)) != nil ){
                  DEBUG_MSG("; Variable already exists in top level environment: modyfing.. ");
                  symbol_value = sfs_eval(car(cdr(cdr(input))));
                  if(symbol_value) {
                    symbol->this.pair.car->this.pair.cdr = symbol_value;
                    return car(car(symbol));
                  }
                  else{
                    return NULL;
                  }

                 }
                 else{
                  symbol_value = sfs_eval(car(cdr(cdr(input))));
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
                symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );

                if(cdr(car(symbol)) != NULL){
                  object old_symbol_value = make_object(cdr(car(symbol))->type);
                  old_symbol_value = cdr(car(symbol));
                  symbol->this.pair.car->this.pair.cdr = sfs_eval(car(cdr(cdr(input))));
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

              if(cdr(cdr(cdr(cdr(input)))) == nil){

                DEBUG_MSG("; \" if \" forme detected");
                if(sfs_eval(car(cdr(input))) != false){
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
                if(sfs_eval(car(input)) == false){
                  return false;
                }
                input = cdr(input);

                if(input == NULL) { /* and without arguments */
                  return true;
                }
              }
                return sfs_eval(car(input));
              }



            /* Implementing or forme evaluation. */
            else if(is_or(caar(input))){
              DEBUG_MSG("; \" or \" forme detected");
              input = cdr(input);
              if(car(input) == NULL){ /* or without arguments */
                return false;
              }
              while(cdr(input) != nil){
                if(sfs_eval(car(input)) != false){
                  return sfs_eval(car(input));
                }
                input = cdr(input);
              }
              return sfs_eval(car(input));
            }

            /* Implementing primitive evaluation. */
            else if(is_primitive(car(input))){
              DEBUG_MSG("; primitive \" %s \" detected", caar(input)->this.symbol);
              primitive primitive_function = cdr(car(input))->this.primitive.function;
              object arguments = nil;
              object reverse_arguments = nil;
              while(cdr(input) != nil){
                object evaluated_argument = sfs_eval(car(cdr(input)));
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

            /* Implementing symbol evaluation. */
            else if(car(input)->type == SFS_SYMBOL){
                object symbol;
                symbol = search_symbol_in_environment( car(input)->this.symbol );
                if(cdr(car(symbol)) != NULL ){
                  DEBUG_MSG("; Variable found in top level environment");
                  return cdr(car(symbol));
                }
                WARNING_MSG("; ERROR: unbound variable: %s", car(car(symbol))->this.symbol );
                return NULL;
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
      return NULL;
    }
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
       || is_if(symbol) || is_and(symbol) || is_or(symbol)){
      return TRUE;
    }
    return FALSE;
}
