/**
 * @file primitive.c
 * @author Rafael Costa
 * @date Sat Nov 12 20:25:33 2016
 * @brief Primitive functions for the schemeC interpreter.
 *
 * Definition of the primitive functions for the schemeC interpreter.
 */

#include <stdio.h>
#include "primitive.h"
#include "print.h"
#include "mem.h"

void initialize_primitive (string symbol, primitive function){
  object symbol_pair = make_symbol(symbol, top_level_environment);
  symbol_pair->this.pair.cdr = make_primitive(function);
}

void initialize_primitives(void){
  DEBUG_MSG("; Initializing primitives in the top level environment..");

  initialize_primitive("null?", is_null_primitive);
  initialize_primitive("boolean?", is_boolean_primitive);
  initialize_primitive("symbol?", is_symbol_primitive);
  initialize_primitive("integer?", is_integer_primitive);
  initialize_primitive("char?", is_char_primitive);
  initialize_primitive("string?", is_string_primitive);
  initialize_primitive("pair?", is_pair_primitive);
  initialize_primitive("procedure?", is_procedure_primitive);

  initialize_primitive("char->integer", char_to_integer_primitive);
  initialize_primitive("integer->char", integer_to_char_primitive);
  initialize_primitive("number->string", number_to_string_primitive);
  initialize_primitive("string->number", string_to_number_primitive);
  initialize_primitive("symbol->string", symbol_to_string_primitive);
  initialize_primitive("string->symbol", string_to_symbol_primitive);

  initialize_primitive("+", integer_addition_primitive);
  initialize_primitive("-", integer_subtraction_primitive);
  initialize_primitive("*", integer_multiplication_primitive);
  initialize_primitive("quotient", integer_quotient_primitive);
  initialize_primitive("remainder", integer_remainder_primitive);
  initialize_primitive("=", integer_equal_primitive);
  initialize_primitive("<", integer_less_primitive);
  initialize_primitive(">", integer_greater_primitive);

  initialize_primitive("cons", cons_primitive);
  initialize_primitive("car", car_primitive);
  initialize_primitive("cdr", cdr_primitive);
  initialize_primitive("list", list_primitive);
  initialize_primitive("set-car!", set_car_primitive);
  initialize_primitive("set-cdr!", set_cdr_primitive);

  initialize_primitive("eq?", polymorphic_equality_primitive);

  DEBUG_MSG("; ------------------------------------------- Completed");
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

object is_procedure_primitive(object arguments){
  if(cdr(arguments) == nil){
    if(car(arguments)->type == SFS_COMPOUND){
      return true;
    }
    return false;
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure procedure? ");
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
      object symbol = make_symbol(buffer, top_level_environment);
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
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer-addition is not of the correct type");
      return NULL;
    }
  }
  return result;
}

object integer_subtraction_primitive(object arguments){
  if(car(arguments)){
    if(car(arguments)->type == SFS_NUMBER){
      object result;
      if(cdr(arguments) == nil){
        result = make_number(-car(arguments)->this.number.this.integer, NUM_INTEGER);
      }
      else{
        result = make_number(car(arguments)->this.number.this.integer, NUM_INTEGER);
      }
      arguments = cdr(arguments);
      while (arguments != nil) {
        object operand = car(arguments);
        if (operand->type == SFS_NUMBER) {
          result->this.number.this.integer -= operand->this.number.this.integer;
          arguments = cdr(arguments);
        }
        else{
          WARNING_MSG("; ERROR: argument passed to primitive procedure integer-subtraction is not of the correct type");
          return NULL;
        }
      }
      return result;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure integer-subtraction is not of the correct type");
      return NULL;
    }
  }
  else{
    WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure integer-subtraction");
    return NULL;
  }

}

object integer_multiplication_primitive(object arguments){
  if(car(arguments)){
    object result = make_number(car(arguments)->this.number.this.integer, NUM_INTEGER);
    arguments = cdr(arguments);
    while (arguments != nil) {
      object operand = car(arguments);
      if (operand->type == SFS_NUMBER) {
        result->this.number.this.integer *= operand->this.number.this.integer;
        arguments = cdr(arguments);
      }
      else{
        WARNING_MSG("; ERROR: argument passed to primitive procedure integer-multiplication is not of the correct type");
        return NULL;
      }
    }
    return result;
  }
  else{
    return make_number(1, NUM_INTEGER);
  }
}

object integer_quotient_primitive(object arguments){
  if(car(arguments)){
    if(cdr(cdr(arguments)) == nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER ){
        int dividend = car(arguments)->this.number.this.integer;
        int divisor = car(cdr(arguments))->this.number.this.integer;
        object result = make_number(dividend/divisor, NUM_INTEGER);
        return result;
      }
      else{
          WARNING_MSG("; ERROR: argument passed to primitive procedure integer-quotient is not of the correct type");
          return NULL;
      }
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure integer-quotient");
  return NULL;
}

object integer_remainder_primitive(object arguments){
  if(car(arguments)){
    if(cdr(cdr(arguments)) == nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER ){
        int dividend = car(arguments)->this.number.this.integer;
        int divisor = car(cdr(arguments))->this.number.this.integer;
        object result = make_number(dividend%divisor, NUM_INTEGER);
        return result;
      }
      else{
          WARNING_MSG("; ERROR: argument passed to primitive procedure integer-remainder is not of the correct type");
          return NULL;
      }
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure integer-remainder");
  return NULL;
}

object integer_equal_primitive(object arguments){
  if(car(arguments) && cdr(arguments) != nil){
    while(cdr(arguments) != nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER){
        if(car(arguments)->this.number.this.integer == car(cdr(arguments))->this.number.this.integer){
          arguments = cdr(arguments);
        }
        else{
          return false;
        }
      }
      else{
        WARNING_MSG("; ERROR: argument passed to primitive procedure integer-equality is not of the correct type");
        return NULL;
      }
    }
    return true;
  }
  else{
    return true;
  }
}

object integer_less_primitive(object arguments){
  if(car(arguments) && cdr(arguments) != nil){
    while(cdr(arguments) != nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER){
        if(car(arguments)->this.number.this.integer < car(cdr(arguments))->this.number.this.integer){
          arguments = cdr(arguments);
        }
        else{
          return false;
        }
      }
      else{
        WARNING_MSG("; ERROR: argument passed to primitive procedure integer-less is not of the correct type");
        return NULL;
      }
    }
    return true;
  }
  else{
    return true;
  }
}

object integer_greater_primitive(object arguments){
  if(car(arguments) && cdr(arguments) != nil){
    while(cdr(arguments) != nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER){
        if(car(arguments)->this.number.this.integer > car(cdr(arguments))->this.number.this.integer){
          arguments = cdr(arguments);
        }
        else{
          return false;
        }
      }
      else{
        WARNING_MSG("; ERROR: argument passed to primitive procedure integer-less is not of the correct type");
        return NULL;
      }
    }
    return true;
  }
  else{
    return true;
  }
}

/* List manipulation primitives */
object cons_primitive(object arguments){
  if(cdr(cdr(arguments)) == nil){
    object pair = make_pair();
    pair->this.pair.car = car(arguments);
    pair->this.pair.cdr = car(cdr(arguments));
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

object set_car_primitive(object arguments){
  if(car(arguments)){
    if(car(arguments)->type == SFS_PAIR){
      object old_car = caar(arguments);
      arguments->this.pair.car->this.pair.car = car(cdr(arguments));
      return old_car;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure set-car! is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure set-car! ");
  return NULL;
}

object set_cdr_primitive(object arguments){
  if(car(arguments)){
    if(car(arguments)->type == SFS_PAIR){
      object old_cdr = cdr(car(arguments));
      arguments->this.pair.car->this.pair.cdr = car(cdr(arguments));
      return old_cdr;
    }
    else{
      WARNING_MSG("; ERROR: argument passed to primitive procedure set-cdr! is not of the correct type");
      return NULL;
    }
  }
  WARNING_MSG("; ERROR: wrong number of arguments given to primitive procedure set-cdr! ");
  return NULL;
}

object list_primitive(object arguments){
  return arguments;
}

/* Polymorphic equality */
object polymorphic_equality_primitive(object arguments){
  if(car(arguments) && cdr(arguments) != nil){
      if(car(arguments)->type == SFS_NUMBER && car(cdr(arguments))->type == SFS_NUMBER){
        if(car(arguments)->this.number.this.integer == car(cdr(arguments))->this.number.this.integer){
          return true;
        }
        else{
          return false;
        }
      }
      else if(car(arguments)->type == SFS_CHARACTER && car(cdr(arguments))->type == SFS_CHARACTER){
        if(car(arguments)->this.character == car(cdr(arguments))->this.character){
          return true;
        }
        else{
          return false;
        }
      }
      else if(car(arguments)->type == SFS_BOOLEAN && car(cdr(arguments))->type == SFS_BOOLEAN){
        if(car(arguments) == car(cdr(arguments))){
          return true;
        }
        else{
          return false;
        }
      }
      else if(car(arguments)->type == SFS_STRING){
          return false;
      }

      else if(!strcmp( car(arguments)->this.symbol, car(cdr(arguments))->this.symbol )){
          return true;
      }
      else{
        return false;
      }
    }
  else{
    return true;
  }
}

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
