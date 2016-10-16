
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

    /* auto-evaluation */
    if(input->type == SFS_NUMBER || input->type == SFS_CHARACTER
       || input->type == SFS_STRING || input->type == SFS_BOOLEAN){
         return input;
       }

    else{

        if(input->type == SFS_PAIR){

          if( is_quote(car(input)) ){
            if( cdr(cdr(input)) == nil ){
              DEBUG_MSG("# quote forme detected");
              return car(cdr(input));
            }
            else{
              WARNING_MSG("Wrong type to apply");
              return NULL;
            }

          }

          else if(is_define(car(input))){
            DEBUG_MSG("# define forme detected");
            object symbol = make_pair();
            symbol->this.pair.car = NULL;
            symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );
            if(cdr(car(symbol)) != nil ){
              DEBUG_MSG("# Variable already exists in top level environment, modyfing.. ");
              symbol->this.pair.car->this.pair.cdr = car(cdr(cdr(input)));
              return car(car(symbol));
            }
            else{
              symbol->this.pair.car->this.pair.cdr = car(cdr(cdr(input)));
              return car(car(symbol));
            }
            return NULL;
          }

          else if(is_set(car(input))){
            DEBUG_MSG("# set! forme detected");
            object symbol = make_pair();
            symbol->this.pair.car = NULL;
            symbol = search_symbol_in_environment( car(car(cdr(input)))->this.symbol );

            if(cdr(car(symbol)) != nil){
              object old_symbol_value = make_object(cdr(car(symbol))->type);
              old_symbol_value = cdr(car(symbol));
              symbol->this.pair.car->this.pair.cdr = car(cdr(cdr(input)));
              return old_symbol_value;
            }
            else{
              WARNING_MSG("Undefined variable");
              return nil;
            }
          }

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
            WARNING_MSG("# Wrong type to apply");
            return input;
          }
      }
      WARNING_MSG("Primitive forme missing arguments");
      return NULL;
    }

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

int is_forme(object symbol){
    if(is_quote(symbol) || is_define(symbol) || is_set(symbol)){
      return TRUE;
    }
    return FALSE;
}
