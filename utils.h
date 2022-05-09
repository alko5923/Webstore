# ifndef __UTILS_H__
# define __UTILS_H__

# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <math.h>
# include <stdlib.h>


extern char *strdup(const char *);

// type definitions
typedef bool(*check_func)(char *); // or char *str?
typedef union {
  int int_value;
  float float_value;
  char *string_value;
}
  answer_t;

typedef answer_t(*convert_func)(char *); // or char *str?


int read_string(char *buf, int buf_size);
bool is_number(char *str);
bool is_float(char *str);
answer_t make_float(char *);
bool not_empty(char *str);
answer_t ask_question(char *question, check_func check, convert_func convert);
char *ask_question_string(char *question);
int ask_question_int(char *question);
double ask_question_float(char *question);
char ask_question_menu(char *valid_answers, char *print_text);
void print_menu();
int print(char *str);
int println(char* str);
char *trim(char *str);
char *ask_question_shelf(char *question);



#endif 
