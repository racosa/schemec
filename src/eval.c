
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
object sfs_eval( object input ) {

  if(false){



    /* auto-evaluation */
    if(input->type == SFS_NUMBER || input->type == SFS_CHARACTER
       || input->type == SFS_STRING || input->type == SFS_BOOLEAN){
         return input;
       }

    else{
      if(input->type == SFS_SYMBOL){
        return 0;
      }
      else if(input->type == SFS_PAIR){
        if(is_quote(car(input))){
          DEBUG_MSG("Quote forme detected");
          return car(cdr(input));
        }
        else if(is_define(car(input))){
          DEBUG_MSG("Define forme detected");
          return input;
        }
        else{
          WARNING_MSG("Wrong type to apply");
          return input;
        }
      }
    }

} return input;

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
