/**
 * @file print.c
 * @author Rafael Costa
 * @date Sat Nov 12 00:00:00 2016
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"
#include "eval.h"
#include <stdio.h>

void sfs_print_atom( object o ) {

    switch (o->type) {

      case SFS_NUMBER:
      /*printf( "SFS_NUMBER: %d", o->this.number.this.integer );*/
        switch (o->this.number.numtype) {
          case NUM_PINFTY:
            printf( "+inf" );
          break;
          case NUM_MINFTY:
            printf( "-inf" );
          break;
          case NUM_INTEGER:
            printf( "%d", o->this.number.this.integer );
          break;
          default:
            printf( "%lf", o->this.number.this.real );
          break;
        }
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
        DEBUG_MSG("Printing symbol...");
        printf( "%s", o->this.symbol );
        DEBUG_MSG("Printing symbol completed.");
      break;

      case SFS_NIL:
        /*printf("SFS_NIL: ()");*/
        printf("()");
      break;
      case SFS_PRIMITIVE:
        printf("#<primitive-procedure>");
      break;
      case SFS_COMPOUND:
        printf("#<procedure>");
      break;
      default:
        WARNING_MSG("EXCEPTION in print_atom() function");
      break;
    }
    return;
}

void sfs_print_pair( object o ) {

    if(car(o)->type == SFS_PAIR && car(car(o))->type != SFS_SYMBOL){ /*Fixing print of symbols that belong to the environment*/
      printf("(");
      sfs_print(car(o));
    }
    else{
      sfs_print(car(o));
    }
    if (o->this.pair.cdr == nil){
      printf(")");
    }
    else {
      printf(" ");
      sfs_print(cdr(o));
    }

    return;
}

void sfs_print( object o ) {

    if ( SFS_PAIR == o->type ) {

    /* TODO  -- Fix "#<primitive-procedure>"
    if(cdr(o)){
        if(cdr(o)->type == SFS_PRIMITIVE){
          sfs_print_atom(cdr(o));
        }
      }
    else
      */
      if(car(o)->type == SFS_SYMBOL){
          sfs_print_atom(car(o));
      }
      else if(cdr(o)->type != SFS_PAIR && cdr(o) != nil){ /*Printing pairs from cons primitive procedure*/
          sfs_print_atom(car(o));
          printf(" . ");
          sfs_print_atom(cdr(o));
          printf(")");
      }
      else{
          sfs_print_pair( o );
      }
    }
    else {
        sfs_print_atom( o );
    }
}
