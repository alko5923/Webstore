# include <stdio.h>
# include <string.h>
# include <math.h>
# include "utils.h"
# include <ctype.h>
# include <stdbool.h>


bool not_empty(char *str)
{
  return strlen(str) > 0;
}


bool is_number(char *str)
{
  if (strlen(str) < 1)
    {
      return false;
    }
  for (int i = 0; i < strlen(str); i+=1)
    {
      if (!isdigit(str[i]))
	{
	  return false;
	}
    }
  return true;
}


bool is_float(char *str)
{
  bool temp = false;
  int counter = 0;
  bool answer = true;

  for (int i = 0; i < strlen(str); i+=1)
    {
      temp = isdigit(str[i]);
      if ((temp == false) && (str[i] != '.'))
	{
	  return false;
	}
      if ((temp == false) && str[i] == '.')
	{
	  counter += 1;
	}
    }

  if ((temp == false) && (counter == 1))
    {
      answer = true;
    }
  else if ((temp == false) && (counter == 0))
    {
      answer = false;
    }
  else if ((temp == true) && (counter == 0))
    {
      answer = false;
    }
  return answer;
}


answer_t make_float(char *str)
{
  answer_t a;
  a.float_value = atof(str);
  return a;
}


void clear_input_buffer()
{
  int c; 
  do
    {
      c = getchar();
    }
  while (c != '\n' && c != EOF);
}


int read_string(char *buf, int buf_size)
{
  int ch;
  int n = 0;
  while ((ch = getchar()) != EOF && ch != '\n' && n < (buf_size - 1))
    {
      buf[n] = ch;
      n += 1;
    }
  buf[n] = '\0';
  if (n == buf_size - 1)
      clear_input_buffer();
  return n;
}


answer_t ask_question(char *question, check_func check, convert_func convert)
{
  int buf_size = 250;
  char buf[buf_size];
  do
    {
      puts(question);
      read_string(buf, buf_size);
    }
  while (!check(buf));
  return convert(buf);
}


int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.int_value;
}




//char *ask_question_string(char *question)
//{
//  return ask_question(question, not_empty, (convert_func) strdup).string_value;
//}

char *ask_question_string(char *question)
{
  char *string = ask_question(question, not_empty, (convert_func) strdup).string_value;
  return string;
}


double ask_question_float(char *question)
{
  return ask_question(question, is_float, make_float).float_value;
}



char ask_question_menu(char *valid_answers, char *text)
{
  print(text);
  char *question = "Please make a choice:";
  int flag_answer = false;
  char accepted_answer;

  while (!flag_answer)
    {
      char *answer = ask_question_string(question);
      if (strpbrk(valid_answers, answer) && strlen(answer) == 1)
	{
	  flag_answer = true;
	  accepted_answer = *answer;
	}
	free(answer);
    }
  
  return toupper(accepted_answer);
}

//char ask_question_menu()
//{
//  print_menu();
//  char *question = "Please make a choice:";
//  int flag_answer = 0;
//  char accepted_answer;
//  char *valid_answers = "AaRrEeUuSsFf";
//
//  while (!flag_answer)
//    {
//      char *answer = ask_question_string(question);
//      if (strpbrk(valid_answers, answer) && strlen(answer) == 1)
//	{
//	  flag_answer = 1;
//	  accepted_answer = *answer;
//	}
//    }
//  return toupper(accepted_answer);
//}

void print_menu(char *print_text)
{
  printf("%s", print_text);
}

//void print_menu()
//{
//  printf("[L]ist all merchandise \n[C]reate cart \n [E]xit \n");
//}

int print(char *str)
{
  int j = 0;
  while (j < strlen(str))
    {
      putchar(str[j]);
      j += 1;
    }
    return 0;
}


int println(char* str)
{
  print(str);
  putchar('\n');

  return 0;
}

char *trim(char *str)
{
  char *start = str;
  char *end = start + strlen(str)-1;

  while (isspace(*start)) ++start;
  while (isspace(*end)) --end;

  char *cursor = str;
  for (; start <= end; ++start, ++cursor)
    {
      *cursor = *start;
    }
  *cursor = '\0';

  return str;
}


bool check_shelf(char *shelf)
{
  int i;
  if (strlen(shelf) < 3 || strlen(shelf) > 3)
    {
      return false;
    }

  if (!isalpha(shelf[0]) || (islower(shelf[0])))
    {
      return false;
    }
  
  for (i = 1; i<strlen(shelf); i+=1)
    {
      if (!isdigit(shelf[i]))
      {
        return false;
      }
    }
  return true;
}


char *ask_question_shelf(char *question)
{
  return ask_question(question, check_shelf, (convert_func) strdup).string_value;
}