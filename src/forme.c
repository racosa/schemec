/**
 * @file forme.c
 * @author Rafael Costa Sales
 * @date Sat Dec 03 18:19:22 2016
 * @brief Forme functions for the schemeC interpreter.
 *
 * Basic forme functions for the schemeC interpreter.
 */

#include "forme.h"

void initialize_formes(void){
  DEBUG_MSG("; Initializing formes in the top level environment..");
  int i;
  const char *formes[] = {
    "quote",
    "define",
    "set!",
    "if",
    "and",
    "or",
    "begin",
    "lambda",
    "let",
    "let*",
    "letrec",
    "map"
  };
  for(i=0 ; i < 6 ; i++){
    object symbol_name = make_object(SFS_SYMBOL);
    object symbol_pair = make_pair();
    symbol_pair->this.pair.car = make_object(SFS_SYMBOL);
    strcpy( symbol_name->this.symbol, formes[i] );
    symbol_pair->this.pair.car = symbol_name;
    insert_symbol_in_environment(symbol_pair, top_level_environment);
  }
  DEBUG_MSG("; ---------------------------------------- Completed");
}

int is_let( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "let" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_let_asterisc( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "let*" ) ){
      return TRUE;
    }
  }
  return FALSE;
}

int is_map( object object ){
  if(object){
    if ( !strcmp(object->this.symbol, "map" ) ){
      return TRUE;
    }
  }
  return FALSE;
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
