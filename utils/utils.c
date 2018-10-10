#include "utils.h"

bool not_empty(char *str)
{
	return strlen(str) > 0;
}

bool is_float(char *str)
{
	bool flag = false;

	for(int i = 0; i < strlen(str); ++i)
	{
		if(!isdigit(str[i]) && str[i] != '.')
		{
			return false;
		}
		else if(!isdigit(str[i]) && str[i] == '.' && !flag)
		{
			flag = true;
		}
		else if(!isdigit(str[i]) && str[i] == '.' && flag)
		{
			return false;
		}
	}
	return true;
}

answer_t make_float(char *str)
{
	  return (answer_t) { .float_value = atof(str) };
}

bool is_amount(char *str)
{
	for(int i = 0; i < strlen(str); ++i)
	{
		if(!isdigit(str[i])) { return false; }
	}
	return true;
}

answer_t make_char(char *str)
{
	return (answer_t) { .int_value = str[0] };
}

bool is_menu_selection(char *str)
{
	char i = str[0];
	if(strlen(str) != 1) { return false; }

	else if(i == 'L' || i == 'l' || i == 'T' || i == 't' || i == 'R' 
			 || i == 'r' || i == 'G' || i == 'g' || i == 'H' 
			 || i == 'h' || i == 'A' || i == 'a') 
	{ return true; }

	else { return false; }
}

bool is_number(char *str)
{
	if(!isdigit(str[0]) && str[0] != '-') { return false; }
	else
	{
		for(int i = 1; i < strlen(str); ++i)
		{
			if(!isdigit(str[i])) { return false; }
		}
		return true;
	}
}

void clear_input_buffer()
{
        char n = 0;

	do
	{
		n = getchar();
	}
	while(n != '\n' && n!= EOF);
}


int read_string(char *buf, int buf_siz)
{
	char c = 0;
	int acc = 0;

	do
	{
		c = getchar();

		if(c == '\n' && c != EOF)
		{
			buf[acc] = '\0';
			return acc;
		}						

		buf[acc] = c;
		acc+=1;

		if(acc == buf_siz)
		{
			clear_input_buffer();
			buf[buf_siz] = '\0';
			return buf_siz;
		}
	}
	while(true);
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
	int buff_size = 36;
	char buff[buff_size];

	do
	{
		printf("%s\n", question);
		read_string(buff, buff_size);

		if(check(buff))
		{
			return convert(buff);	
		}
	}
	while(true);
}	

char *ask_question_string(char *question)
{
	return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

int ask_question_int(char *question)
{
	answer_t answer = ask_question(question, is_number, (convert_func) atoi);
	return answer.int_value;
}

double ask_question_float(char *question)
{
	return ask_question(question, is_float, make_float).float_value;
}
