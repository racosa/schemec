
/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#include <stdio.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "read.h"
#include "mem.h"
#include "print.h"


void flip( uint *i ) {

    if ( *i == FALSE ) {
        *i = TRUE;
    }
    else {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line) {

    int i=0;
    if (line == NULL) {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i])) {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0') {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';') {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 * 				- la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum {
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp ) {
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do {
        ret = NULL;
        chunk = NULL;

        /* si en mode interactif*/
        if ( stdin == fp ) {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ ) {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0) {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            chunk = readline( sfs_prompt );
        }
        /*si en mode fichier*/
        else {
            chunk=k;
            memset( chunk, '\0', BIGSTRING );
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret ) {
                /* fin de fichier...*/
                if ( parlevel != 0 ) {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp)) {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL) {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 ) {
            if (strlen(input) + s > BIGSTRING-1 ) {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ ) {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL ) {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i]) {
                case '(':
                    if (in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel--;
                        if (parlevel == 0 ) {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 ) {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i<2 || chunk[i-1] != '\\' ) {
                        if ( in_string == FALSE ) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        }
                        else {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE) {
                        if(isspace(chunk[i])) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        } else if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED) {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL) {
                        if(*first_useful == ')' ) {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }
                        else {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE) {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin ) {
            if ( feof( fp ) ) {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    } while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    if(stdin == fp) {
        add_history( input );
    }
    return S_OK;
}

object sfs_read( char *input, uint *here ) {

    if ( input[*here] == '(' ) {
        if ( input[(*here)+1] == ')' ) {
            *here += 2;
            DEBUG_MSG("Atome identified of type: SFS_NIL -> Value: () " );
            return nil;
        }
        else {
            (*here)++;
            return sfs_read_pair( input, here );
        }
    }
    else {
        return sfs_read_atom( input, here );
    }
}

object sfs_read_atom( char *input, uint *here ) {

    object atom;
    int state = STATE_INIT;
    int number_input = 0;
    char *atom_input;
    uint atom_size = 0;

    uint here_init = *here;
    uint atom_found = FALSE;

    atom_input = NULL;

    while ( !atom_found ){
      switch ( state ) {

        case STATE_INIT:
          if( input[ (*here) ] == ' ' ){
            state = STATE_INIT;
            DEBUG_MSG("STATE_INIT");
          }
          else if( input[ (*here) ] == '(' && input[ (*here) + 1 ] == ')' ){
            state = STATE_EMPTY_LIST;
          }

          else if( input[ (*here) ] == '+' || input[ (*here) ] == '-' ){
            state = STATE_NUMBER;
          }
          else if( input[ (*here) ] >= '0' && input[ (*here) ] <= '9'){
            state = STATE_NUMBER;
          }
          else if( input[ (*here) ] == '\"' ){
            state = STATE_CHAINE_CHAR;
          }
          else if( input[ (*here) ] == '#'){
            state = STATE_BOOLEAN;
          }
          else if ( input[ (*here) ] != ')' && input[ (*here) ] != ' ' && input[ (*here) ] != 0 ){
            state = STATE_SYMBOL;
          }
          break;

        case STATE_NUMBER:
          if( input [ (*here) ] == '\0' || input [ (*here) ] == ' ' ){
            atom_size = (*here)- here_init;
            atom_input = sfs_malloc( (atom_size)*sizeof( char ) );
            memmove(atom_input, (input + here_init), atom_size);
            number_input = atoi(atom_input);
            atom = make_integer(number_input);
            DEBUG_MSG("Atome identified of type: SFS_NUMBER -> Value: %d ", atom->this.number.this.integer );
            sfs_free(atom_input);
            atom_found = TRUE;
          }
          break;

        case STATE_CHAINE_CHAR:
          if( input [ (*here) ] == '\"' && (input [ (*here) + 1 ] == '\0' || input [ (*here) + 1 ] == ' ' ) ){
            atom_size = (*here)- here_init + 1;
            atom_input = sfs_malloc( (atom_size)*sizeof( char ) );
            memmove( atom_input, &input[here_init], atom_size );
            atom = make_string( atom_input );
            DEBUG_MSG("Atome identified of type: SFS_STRING -> Value: %s ", atom->this.string );
            sfs_free(atom_input);
            atom_found = TRUE;
          }
          break;

        case STATE_BOOLEAN:
          if(input[ (*here)+1 ] == '\0' || input [ (*here)+1 ] == ' '){
            if( input[ (*here) ] == 't' ){
              atom = true;
              DEBUG_MSG("Atome identified of type: SFS_BOOLEAN -> Value: true " );
              atom_found = TRUE;
            }
            else if ( input[ (*here) ] == 'f' ){
              atom = false;
              DEBUG_MSG("Atome identified of type: SFS_BOOLEAN -> Value: false " );
              atom_found = TRUE;
            }
          }
          else if ( input[ (*here) ] == '\\' ) {
            state = STATE_CHAR;
          }
          else {
            WARNING_MSG("ERROR: invalid atom type");
            return 0;
          }
          break;

        case STATE_CHAR:
          if( !strcmp( input, "#\\space" ) && ( input[ (*here)+6 ] == '\0' || input [ (*here) ] == ' ' ) ){
            atom = make_character( ' ' );
            DEBUG_MSG("Atome identified of type: SFS_CHARACTER -> Value: #\\space " );
            atom_found = TRUE;
          }
          else if( !strcmp( input, "#\\newline" ) && ( input[ (*here)+8 ] == '\0' || input [ (*here) ] == ' ' ) ){
            atom = make_character( '\n' );
            DEBUG_MSG("Atome identified of type: SFS_CHARACTER -> Value: #\\newline " );
            atom_found = TRUE;
          }
          else if( input[ (*here) ] != '\0' && ( input[ (*here)+1 ] == '\0' || input [ (*here)+1 ] == ' ' ) ){
            atom = make_character( input[ (*here) ] );
            DEBUG_MSG("Atome identified of type: SFS_CHARACTER -> Value: %c ", atom->this.character );
            atom_found = TRUE;
          }
          else if ( input[ (*here) ] != '\0' || input [ (*here) ] == ' ' ){
            WARNING_MSG("ERROR: invalid atom type");
            return 0;
          }
          break;

        case STATE_SYMBOL:
          if( input[ (*here) ] == '\0' || input [ (*here) ] == ' ' ){
            atom_input = sfs_malloc(((*here)-(here_init)+1)*sizeof( char ));
            memmove(atom_input, (input+(here_init)), (*here)-(here_init));
            atom = make_symbol( atom_input );
            DEBUG_MSG("Atome identified of type: SFS_SYMBOL -> Value: %s ", atom->this.symbol );
            free(atom_input);
            atom_found = TRUE;
          }
          break;

        case STATE_EMPTY_LIST:
          atom = nil;
          DEBUG_MSG("Atome identified of type: SFS_NIL -> Value: () " );
          atom_found = TRUE;
        break;

        default:
          WARNING_MSG("ERROR: invalid atom type");
          return 0;
          break;
      }
      (*here)++;

    }
    return atom;
}

object sfs_read_pair( char *stream, uint *i ) {

    object pair = make_pair();
    object next_object;

    while (*i < strlen(stream)) {
      if (isspace(stream[*i])){
          (*i)++;
      }
      DEBUG_MSG("i = %d", *i);

      if (stream[*i] == ')') {
        DEBUG_MSG("return pair");
        (*i)++;
        return pair;
      }
      else {
        next_object = sfs_read( stream, i );
        if (next_object){
          DEBUG_MSG("insert list");
          insert_linked_list( next_object, pair );
        }
        else{
          DEBUG_MSG("return 0");
          return 0;
        }
      }
    }
    return 0;
}


void insert_linked_list(object car, object list){

    if (list->this.pair.car == NULL){
      list->this.pair.car = car;
    /*  return list; */
    }
    else{
    /*
      object new_pair = make_pair();
      new_pair->this.pair.car = car;
      new_pair->this.pair.cdr = list->this.pair.cdr;
      list->this.pair.cdr = new_pair;
      */
      object new_pair = make_pair();
      while(true){
        if ( list->this.pair.cdr == nil ){
          new_pair->this.pair.car = car;
          list->this.pair.cdr = new_pair ;
          break;
        }
        list = list->this.pair.cdr;
      }
      /*return new_pair;*/
    }
}
