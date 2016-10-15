
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

    object t = make_object( SFS_SYMBOL );

    strcpy( t->this.symbol, symbol );

    return t;
}

object make_pair ( void ){

    object t = make_object ( SFS_PAIR );
    t->this.pair.cdr = nil;

    return t;
}
