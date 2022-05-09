#pragma once

#include <stdbool.h>
#include <stdlib.h>


typedef union elem elem_t;
typedef struct option option_t;


union elem
{
  int number;
  unsigned int p_number;
  bool b_ool;
  float f_number;
  void *pointer;
  char *str;
};

struct option
{
  bool success;
  elem_t value;
};



/// @brief creates an elem_t out of an int
/// @param int_value value to be converted
/// @return elem_t elem_t version of int_value
elem_t ioopm_make_elem(int int_value);

/// @brief compares two integers
/// @param int_1 first int to compare
/// @param int_2 second int to compare
/// @retrun true if int_1 == int_2 else false
bool int_key_eq(elem_t int_1, elem_t int_2);

/// @brief compares two strings
/// @param string_1 first string to compare
/// @param string_2 second string to compare
/// @retrun true if string_1 == string_2 else false
bool str_key_eq(elem_t string_1, elem_t strin_2);

