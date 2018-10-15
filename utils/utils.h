#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

extern char *strdup(const char *);


typedef union { // Not: undvik gärna så här korta namn på variabler! Varför?
	int   int_value;
	float float_value;
	char *string_value;
	char  char_value;
} answer_t;

typedef bool(*check_func)(char *);

typedef answer_t(*convert_func)(char *);

bool not_empty(char *str);
bool is_number(char *str);
bool is_float(char *str);
bool is_menu_selection(char *str);

int read_string(char *buf, int buf_siz);
int ask_question_int(char *question);
int ask_question_amount(char *question);
char *ask_question_string(char *question);
double ask_question_float(char *question);

answer_t ask_question(char *question, check_func check, convert_func convert);
answer_t make_float(char *);

#endif
