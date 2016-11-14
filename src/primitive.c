/**
 * @file primitive.c
 * @author Rafael Costa Sales
 * @date Sat Nov 12 20:25:33 2016
 * @brief Primitive functions for the schemeC interpreter.
 *
 * Definition of the primitive functions for the schemeC interpreter.
 */

#include <stdio.h>
#include "primitive.h"

void initialize_primitive (string symbol, primitive function){
  object symbol_pair = make_symbol(symbol);
  symbol_pair->this.pair.cdr = make_primitive(function);
  insert_symbol_in_environment(symbol_pair);
}

/* Predicate primitives */
object is_null_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments) == nil){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure null? ");
  return NULL;
}

object is_boolean_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_BOOLEAN){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure boolean? ");
  return NULL;
}

object is_symbol_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_SYMBOL){
      return true;
    }
    else if(car(arguments)->type == SFS_PAIR){
      if(caar(arguments)->type == SFS_SYMBOL){
        return true;
      }
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure symbol? ");
  return NULL;
}

object is_integer_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_NUMBER){
      if(car(arguments)->this.number.numtype == NUM_INTEGER){
        return true;
      }
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure integer? ");
  return NULL;
}

object is_char_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_CHARACTER){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure char? ");
  return NULL;
}

object is_string_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_STRING){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure string? ");
  return NULL;
}

object is_pair_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_PAIR){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure pair? ");
  return NULL;
}

/* Type convertion primitives */
object char_to_integer_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_CHARACTER){
      object integer = make_number(car(arguments)->this.character, NUM_INTEGER);
      return integer;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure char->integer is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure char->integer ");
  return NULL;
}

object integer_to_char_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_NUMBER){
      object character = make_character(car(arguments)->this.number.this.integer);
      return character;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer->char is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure integer->char ");
  return NULL;
}

object number_to_string_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_NUMBER){
      int number = car(arguments)->this.number.this.integer;
      int number_size = calculate_number_size(number);
      string buffer;
      init_string(buffer);
      strncpy(buffer, "\"", 2);
      sprintf(buffer + 1, "%d", number);
      strncpy(buffer + 1 + number_size, "\"", 2);
      object string = make_string(buffer);
      return string;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure number->string is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure number->string ");
  return NULL;
}

object string_to_number_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_STRING){
      char *endptr;
      long integer;
      errno = 0;
      integer = strtol( car(arguments)->this.string + 1, &endptr, 10 );
      if(*endptr != '\"' || (errno != 0 && integer == 0)) {
        return false;
      }
      object number = make_number(integer, NUM_INTEGER);
      return number;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure string->number is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure string->number ");
  return NULL;

}
object symbol_to_string_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_PAIR){
      if(caar(arguments)->type == SFS_SYMBOL){
        string buffer;
        init_string(buffer);
        strncpy(buffer, "\"", 2);
        strcpy(buffer + 1, caar(arguments)->this.symbol);
        strncpy(buffer + 1 + strlen(caar(arguments)->this.symbol), "\"", 2);
        object string = make_string(buffer);
        return string;
      }
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure symbol->string is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure symbol->string ");
  return NULL;
}

object string_to_symbol_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_STRING){
      int i = 0;
      string buffer;
      init_string(buffer);
      strcpy(buffer, car(arguments)->this.string + 1);
      while(buffer[i] != '\0'){
        i++;
      }
      buffer[i-1] = '\0';
      object symbol = make_symbol(buffer);
      return symbol;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure string->symbol is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure string->symbol ");
  return NULL;
}

/* Integer arithmetic primitives */
object integer_addition_primitive(object arguments){
  object result = make_number(0, NUM_INTEGER);
  while (arguments != nil) {
    object operand = car(arguments);
    if (operand->type == SFS_NUMBER) {
      result->this.number.this.integer += operand->this.number.this.integer;
      arguments = cdr(arguments);
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer-add is not of the correct type");
      return NULL;
    }
  }
  return result;
}

object integer_subtraction_primitive(object arguments);
object integer_multiplication_primitive(object arguments);
object integer_quotient_primitive(object arguments);
object integer_remainder_primitive(object arguments);
object integer_equal_primitive(object arguments);
object integer_less_primitive(object arguments);
object integer_greater_primitive(object arguments);

/* List manipulation primitives */
object cons_primitive(object arguments){
  if(cdr(cdr(arguments)) == nil){
    object pair = make_pair();
    pair->this.pair.car = car(cdr(arguments));
    pair->this.pair.cdr = car(arguments);
    return pair;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure cons ");
  return NULL;
}

object car_primitive(object arguments){
  if(caar(arguments)){
    return caar(arguments);
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure car ");
  return NULL;
}

object cdr_primitive(object arguments){
  if(caar(arguments)){
    return cdr(car(arguments));
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure cdr ");
  return NULL;
}

object set_car_primitive(object arguments);
object set_cdr_primitive(object arguments);
object list_primitive(object arguments);

/* Polymorphic equality */
object polymorphic_equality_primitive(object arguments);

int calculate_number_size(int number){
  int negative_number = 0;
  if(number < 0) number = (number == INT_MIN) ? INT_MAX : -number, negative_number++;
  if(number < 10) return 1 + negative_number;
  if(number < 100) return 2 + negative_number;
  if(number < 1000) return 3 + negative_number;
  if(number < 10000) return 4 + negative_number;
  if(number < 100000) return 5 + negative_number;
  if(number < 1000000) return 6 + negative_number;
  if(number < 10000000) return 7 + negative_number;
  if(number < 100000000) return 8 + negative_number;
  if(number < 1000000000) return 9 + negative_number;
  return 10;
}
