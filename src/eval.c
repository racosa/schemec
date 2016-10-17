
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

object sfs_eval( object input ) {
    eval:
    /* Implementing auto-evaluation */
    if(input->type == SFS_NUMBER || input->type == SFS_CHARACTER
       || input->type == SFS_STRING || input->type == SFS_BOOLEAN){
         return input;
       }

    else{

        if(input->type == SFS_PAIR){

          /* Implementing quote forme evaluation. */
          if( is_quote(car(input)) ){
            if( cdr(cdr(input)) == nil ){
              DEBUG_MSG("# \" quote \" forme detected");
              return car(cdr(input));
            }
            else{
              WARNING_MSG("Primitive forme with missing or extra arguments");
              return NULL;
            }
          }

          /* Implementing define forme evaluation. */
          else if(is_define(car(input))){
            DEBUG_MSG("# \" define \" forme detected");
            object symbol = make_pair();
            symbol->this.pair.car = NULL;
            symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );
            if(cdr(car(symbol)) != nil ){
              DEBUG_MSG("# Variable already exists in top level environment, modyfing.. ");
              symbol->this.pair.car->this.pair.cdr = sfs_eval(car(cdr(cdr(input))));
              return car(car(symbol));
            }
            else{
              symbol->this.pair.car->this.pair.cdr = sfs_eval(car(cdr(cdr(input))));
              return car(car(symbol));
            }
            return NULL;
          }

          /* Implementing set! forme evaluation. */
          else if(is_set(car(input))){
            DEBUG_MSG("# \" set! \" forme detected");
            object symbol = make_pair();
            symbol->this.pair.car = NULL;
            symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );

            if(cdr(car(symbol)) != nil){
              object old_symbol_value = make_object(cdr(car(symbol))->type);
              old_symbol_value = cdr(car(symbol));
              symbol->this.pair.car->this.pair.cdr = sfs_eval(car(cdr(cdr(input))));
              return old_symbol_value;
            }
            else{
              WARNING_MSG("Undefined variable");
              return nil;
            }
          }

          /* Implementing if forme evaluation. */
          else if(is_if(car(input))){
            if(cdr(cdr(cdr(cdr(input)))) == nil){
              DEBUG_MSG("# \" if \" forme detected");
              if(sfs_eval(car(cdr(input))) != false){
                DEBUG_MSG("# (predicate) is (true), evaluating (consequence)");
                input = car(cdr(cdr(input)));
              }
              else{
                DEBUG_MSG("# (predicate) is (false), evaluating (alternative)");
                input = car(cdr(cdr(cdr(input))));
              }
              /*Goto begin of eval function*/
              goto eval;
            }
          }

          /* Implementing and forme evaluation. */
          else if(is_and(car(input))){
            DEBUG_MSG("# \" and \" forme detected");
            input = cdr(input);
            while(cdr(input) != nil){
              if(sfs_eval(car(input)) == false){
                return false;
              }
              input = cdr(input);
            }
            return sfs_eval(car(input));
          }

          /* Implementing or forme evaluation. */
          else if(is_or(car(input))){
            DEBUG_MSG("# \" or \" forme detected");
            input = cdr(input);
            while(cdr(input) != nil){
              if(sfs_eval(car(input)) != false){
                return sfs_eval(car(input));
              }
              input = cdr(input);
            }
            return sfs_eval(car(input));
          }

          /* Implementing = operand evaluation. */
          /* TODO Implement multiple arguments evaluation */
          else if(is_equal(car(input))){
            DEBUG_MSG("# \" = \" signal detected");
            input = cdr( input );
            if ( cdr(cdr( input )) == nil ){
              if ( (sfs_eval(car( input )))->this.number.this.integer ==
              (sfs_eval(car(cdr( input ))))->this.number.this.integer){
                return true;
              }
              else{
                return false;
              }
            }
            else{
              WARNING_MSG("Invalid math statement");
              return NULL;
            }
          }

          /* Implementing < signal evaluation. */
          /* TODO Implement multiple arguments evaluation */
          else if(is_smaller(car(input))){
            DEBUG_MSG("# \" < \" signal detected");
            input = cdr( input );
      			if ( cdr(cdr( input )) == nil ){
      				if ( (sfs_eval(car( input )))->this.number.this.integer <
              (sfs_eval(car(cdr( input ))))->this.number.this.integer){
      					return true;
      				}
      				else{
      					return false;
      				}
      			}
      			else{
      				WARNING_MSG("Invalid math statement");
      				return NULL;
      			}
          }

          /* Implementing > signal evaluation. */
          /* TODO Implement multiple arguments evaluation */
          else if(is_bigger(car(input))){
            DEBUG_MSG("# \" > \" signal detected");
            input = cdr( input );
      			if ( cdr(cdr( input )) == nil ){
      				if ( (sfs_eval(car( input )))->this.number.this.integer >
              (sfs_eval(car(cdr( input ))))->this.number.this.integer){
      					return true;
      				}
      				else{
      					return false;
      				}
      			}
      			else{
      				WARNING_MSG("Invalid math statement");
      				return 0;
      			}
          }

          /* TODO Implement addition, subtraction, multiplication and division evaluation */

            /* Implementing symbol evaluation. */
            else if(car(input)->type == SFS_SYMBOL){
              object symbol;
              symbol = search_symbol_in_environment( car(input)->this.symbol );
              if(cdr(car(symbol)) != nil  ){
                DEBUG_MSG("# Variable found in top level environment");
                return cdr(car(symbol));
              }
              WARNING_MSG("# Unbound variable: %s", car(car(symbol))->this.symbol );
              return NULL;
            }

            else{
              WARNING_MSG("Primitive forme with missing or extra arguments");
              return NULL;
            }

          }
          else{
            WARNING_MSG("# Wrong type to apply");
            return NULL;
          }
      }
      WARNING_MSG("Primitive forme with missing or extra arguments");
      return NULL;
    }



int is_quote( object object ){
  if ( !strcmp( object->this.symbol, "quote" ) || !strcmp( object->this.symbol, "\'" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_define( object object ){
  if ( !strcmp(object->this.symbol, "define" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_set( object object ){
  if ( !strcmp(object->this.symbol, "set!" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_if( object object ){
  if ( !strcmp(object->this.symbol, "if" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_and( object object ){
  if ( !strcmp(object->this.symbol, "and" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_or( object object ){
  if ( !strcmp(object->this.symbol, "or" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_equal( object object ){
  if ( !strcmp(object->this.symbol, "=" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_smaller( object object ){
  if ( !strcmp(object->this.symbol, "<" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_bigger( object object ){
  if ( !strcmp(object->this.symbol, ">" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_addition( object object ){
  if ( !strcmp(object->this.symbol, "+" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_subtraction( object object ){
  if ( !strcmp(object->this.symbol, "-" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_multiplication( object object ){
  if ( !strcmp(object->this.symbol, "*" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_division( object object ){
  if ( !strcmp(object->this.symbol, "/" ) ){
    return TRUE;
  }
  return FALSE;
}

int is_forme(object symbol){
    if(is_quote(symbol) || is_define(symbol) || is_set(symbol)
       || is_if(symbol) || is_and(symbol) || is_or(symbol)
       || is_equal(symbol) || is_smaller(symbol) || is_bigger(symbol)
       || is_addition(symbol) || is_subtraction(symbol) || is_multiplication(symbol)
       || is_division(symbol)){

      return TRUE;
    }
    return FALSE;
}
