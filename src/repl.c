
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

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF,SCRIPT} inter_mode;


void usage_error( char *command ) {

    fprintf( stderr, "Usage: %s [file.scm]\n   If no file is given, executes in Shell mode.\n", command );
}

object environment;
object nil;
object true;
object false;

void init_interpreter ( void ) {

    environment = make_top_level_environment();
    nil   = make_nil();
    true  = make_boolean();
    false = make_boolean();

    initialize_formes();
    initialize_primitives();

}

int main ( int argc, char *argv[] ) {

    char     input[BIGSTRING];
    uint     here = 0;
    object   output = NULL;
    object   sexpr = NULL;
    inter_mode mode;
    FILE *   fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche
    WARNING_MSG("Un message WARNING_MSG !");*/

    /* macro INFO_MSG : uniquement si compil avec -DVERBOSE. Cf Makefile*/
    INFO_MSG(" Scheme interpreter - version 0.2 ");
    INFO_MSG(" Press ctrl+c to Exit ");
    INFO_MSG("-----------------------------------");
    /* macro DEBUG_MSG : uniquement si compil avec -DDEBUG (ie : compil avec make debug). Cf Makefile
    DEBUG_MSG("Un message DEBUG_MSG !");*/

    /* La ligne suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */

    if ( argc > 2 ) {
        usage_error( argv[0] );
        exit( EXIT_FAILURE );
    }
    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_error( argv[0] );
        exit( EXIT_SUCCESS );
    }

    init_interpreter();

    /*par defaut : mode shell interactif */
    fp = stdin;
    mode = INTERACTIF;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        mode = SCRIPT;
    }


    while ( 1 ) {
        input[0]='\0';
        here = 0;
        output = NULL;
        sexpr = NULL;
        uint Sexpr_err;

        Sexpr_err = sfs_get_sexpr( input, fp );

        if ( S_OK != Sexpr_err) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                if (Sexpr_err==S_END) {
                    /* Cas fin de fichier script */
                    exit(EXIT_SUCCESS);
                }
                /* Cas S-Expression mal formee dans le fichier script */
                ERROR_MSG("Malformed S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue;
        }

        if ( 0 == strlen( input ) ) {
            continue;
        }

        /* Pour le step 0 on se contente d'afficher */
        /* la S-Expression lue */
        /* puis d'en lire une autre */
        /* METTRE EN COMMENTAIRE LES DEUX LIGNES SUIVANTES */
        /* POUR PASSER A L'INCREMENT 1 */
        /*printf("%s\n", input );*/
        /*continue;*/

        here  = 0;
        DEBUG_MSG("---------------------- Reading Sexpr (...) ----------------------");
        sexpr = sfs_read( input, &here );
        DEBUG_MSG("---------------------- Reading completed ------------------------");
        DEBUG_MSG(".                                                                 ");
        if ( NULL == sexpr ) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Invalid S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }
        DEBUG_MSG("---------------------- Evaluating Sexpr (...) -------------------");
        output = sfs_eval( sexpr );
        DEBUG_MSG("---------------------- Evaluation completed ---------------------");
        DEBUG_MSG(".                                                                 ");
        if( NULL == output) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Error while evaluating input --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
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
