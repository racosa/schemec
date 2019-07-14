
/**
 * @file repl.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:13:02 2012
 * @brief Main REPL for SFS.
 *
 * Main REPL for SFS.
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environment.h"
#include "object.h"
#include "read.h"
#include "eval.h"
#include "print.h"

typedef enum {INTERACTIF,SCRIPT,LIB} inter_mode;


void usage_error( char *command ) {

    fprintf( stderr, "Usage: %s [file.scm]\n   If no file is given, executes in Shell mode.\n", command );
}

object top_level_environment;
/*object current_environment;*/
object nil;
object true;
object false;

void init_interpreter ( void ) {

    nil   = make_nil();
    true  = make_boolean();
    false = make_boolean();
    make_top_level_environment();
    initialize_formes();
    initialize_primitives();
}

int main ( int argc, char *argv[] ) {

    char     input[BIGSTRING];
    uint     here = 0;
    object   output = NULL;
    object   sexpr = NULL;
    inter_mode mode;
    FILE *   fp = NULL;

    INFO_MSG(" schemeC - version 0.4 ");
    INFO_MSG(" Just another Scheme interpreter - Press ctrl+c to exit");
    INFO_MSG("-------------------------------------------------------");

    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_error( argv[0] );
        exit( EXIT_SUCCESS );
    }

    init_interpreter();

    fp = stdin;
    mode = INTERACTIF;
    if(argc == 2) {
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        mode = SCRIPT;
    }

    if(argc == 3 && strcmp(argv[1], "-lib") == 0) {
        INFO_MSG("------ Library detected: uploading lib functions: -----");
        fp = fopen( argv[2], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        mode = LIB;
    }

    while ( 1 ) {
        input[0]='\0';
        here = 0;
        output = NULL;
        sexpr = NULL;
        uint Sexpr_err;

        Sexpr_err = sfs_get_sexpr( input, fp );

        if ( S_OK != Sexpr_err) {

            if(mode == LIB){
              fclose( fp );
              if (Sexpr_err==S_END) {
                  /* End of lib file */
                  mode = INTERACTIF;
                  fp = stdin;
                  INFO_MSG("------------ Library uploaded successfully -----------");
              }
            }

            if (mode == SCRIPT) {
                fclose( fp );
                if (Sexpr_err==S_END) {
                    exit(EXIT_SUCCESS);
                }
                ERROR_MSG("Malformed S-expression --- Aborts");
            }
            continue;
        }

        if ( 0 == strlen( input ) ) {
            continue;
        }
        here  = 0;
        DEBUG_MSG("---------------------- Reading Sexpr (...) ----------------------");
        sexpr = sfs_read( input, &here );
        DEBUG_MSG("---------------------- Reading completed ------------------------");
        DEBUG_MSG(".                                                                 ");
        if ( NULL == sexpr ) {
            if (mode == SCRIPT) {
                fclose( fp );
                ERROR_MSG("Invalid S-expression --- Aborts");
            }
            continue ;
        }
        DEBUG_MSG("---------------------- Evaluating Sexpr (...) -------------------");
        output = sfs_eval( sexpr, top_level_environment );
        DEBUG_MSG("---------------------- Evaluation completed ---------------------");
        DEBUG_MSG(".                                                                 ");
        if( NULL == output) {
            if (mode == SCRIPT) {
                fclose( fp );
                ERROR_MSG("Error while evaluating input --- Aborts");
            }
            continue ;
        }
        DEBUG_MSG("---------------------- Printing result (...) --------------------");
        printf( "==> " );

        /*Fixing print of symbols that belong to the environment*/
        if(output->type == SFS_PAIR && car(output)->type != SFS_SYMBOL){
          printf("(");
        }

        sfs_print( output );
        printf( "\n" );
    }

    if (mode == SCRIPT) {
        fclose( fp );
    }
    exit( EXIT_SUCCESS );
}
