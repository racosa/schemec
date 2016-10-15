
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>

void sfs_print_atom( object o ) {

    switch (o->type) {

      case SFS_NUMBER:
        /*printf( "SFS_NUMBER: %d", o->this.number.this.integer );*/
        printf( "%d", o->this.number.this.integer );
      break;

      case SFS_CHARACTER:
        if( o->this.character == ' ' ){
          /*printf( "SFS_CHARACTER: #\\space " );*/
          printf( "#\\space" );
        }
        else if( o->this.character == '\n' ) {
          /*printf( "SFS_CHARACTER: #\\newline " );*/
          printf( "#\\newline" );
        }
        else{
          /*printf( "SFS_CHARACTER: %c", o->this.character );*/
          printf( "#\\%c", o->this.character );
        }
      break;

      case SFS_STRING:
        /*printf( "SFS_STRING: %s", o->this.string );*/
        printf( "%s", o->this.string );
      break;

      case SFS_BOOLEAN:
        if( o == true ){
            /*printf( "SFS_BOOLEAN: #t" );*/
            printf( "#t" );
        }
        else if( o == false ){
            /*printf( "SFS_BOOLEAN: #f" );*/
            printf( "#f" );
        }
      break;

      case SFS_SYMBOL:
        /*printf( "SFS_SYMBOL: %s", o->this.symbol );*/
        printf( "%s", o->this.symbol );
      break;

      case SFS_NIL:
        /*printf("SFS_NIL: ()");*/
        printf("()");
      break;
      default:
        ERROR_MSG("EXCEPTION in print_atom() function");
      break;
    }
    return;
}

void sfs_print_pair( object o ) {

    if(o->this.pair.car->type == SFS_PAIR ){
      printf("(");
      sfs_print(o->this.pair.car);
    }
    else{
      sfs_print(o->this.pair.car);
    }
    if (o->this.pair.cdr == nil){
      printf(")");
    }
    else {
      printf(" ");
      sfs_print(o->this.pair.cdr);
    }

    return;
}

void sfs_print( object o ) {
    if ( SFS_PAIR == o->type ) {
        sfs_print_pair( o );
    }
    else {
        sfs_print_atom( o );
    }
}
