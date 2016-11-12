
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include "object.h"
#include "mem.h"
#include "environment.h"
#include "eval.h"


object make_object( uint type ) {

    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;

    return t;
}

object make_nil( void ) {

    object t = make_object( SFS_NIL );

    t->this.special = t;

    return t;
}

object make_boolean( void ){

    object t = make_object( SFS_BOOLEAN );

    t->this.boolean = t;

    return t;
}

object make_number( double number, int num_type ) {

    object t = make_object( SFS_NUMBER );

    t->this.number.numtype = num_type;
    if(num_type == NUM_INTEGER){
      t->this.number.this.integer = (int) number;
    }
    else{
      t->this.number.this.real = number;
    }
    return t;
}

object make_character( char character ){

    object t = make_object( SFS_CHARACTER );

    t->this.character = character;

    return t;
}

object make_string ( string string ){

    object t = make_object ( SFS_STRING );

    strcpy( t->this.string, string );

    return t;
}

object make_symbol ( string symbol ){

    object symbol_name = make_object(SFS_SYMBOL);
    strcpy( symbol_name-> this.symbol, symbol );
    object symbol_index = make_pair();
    object symbol_pair = make_pair();
    symbol_pair->this.pair.car = make_object(SFS_SYMBOL);
    symbol_pair->this.pair.car = symbol_name;
    symbol_index->this.pair.car = make_pair();
    symbol_index->this.pair.car = symbol_pair;

    if(car(environment) != NULL){
      symbol_index = search_symbol_in_environment( symbol );
      if (symbol_index){
        return car(symbol_index);
      }
    }
    symbol_index = insert_symbol_in_environment( symbol_pair );
    return car(symbol_index);
}

object make_pair ( void ){

    object t = make_object ( SFS_PAIR );
/*    t->this.pair.car = NULL;
*/    t->this.pair.cdr = nil;

    return t;
}

object car ( object object ){
  if(object){
    if (object->type == SFS_PAIR){
      return object->this.pair.car;
    }
  }
  return NULL;
}

object cdr ( object object ){
  if(object){
    if (object->type == SFS_PAIR){
      return object->this.pair.cdr;
    }
  }
  return NULL;
}

object caar ( object object ){
  if(object){
    if (object->type == SFS_PAIR){
      if(car(object)){
        if (car(object)->type == SFS_PAIR){
          return car(object)->this.pair.car;
        }
      }
    }
  }
  DEBUG_MSG("caar returning NULL...");
  return NULL;
}
