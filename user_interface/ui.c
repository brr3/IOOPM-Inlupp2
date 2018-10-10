#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../logic/logic.h"
#include <stdbool.h>


char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}


char ask_question_menu(char *question)
{
  return toupper(*ask_question(question, is_menu_key, (convert_func) strdup).string_value);
}	


void event_loop(ioopm_hash_table_t *items)
{
  while (true)
    {
      printf("--------------------- \n");

      char key = ask_question_menu("[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista [h]ela varukatalogen\n\
[A]vsluta\n");

      if (key == 'L')
        {
          add_merch_to_db(items); // KLAR  
        }
      /*if (key == 'T')
        {
          remove_item_from_db(items, no_items);
        }
      if (key == 'R')
        {
          if (no_items < 1)
            {
              printf("--------------------- \n");
              printf("OBS! No items exist in database.\n");
            } else
            {
              *items = edit_db(items, no_items);
            }
        }
      if (key == 'G')
        {
          printf("--------------------- \n");
          printf("Not yet implemented!\n");         
        }
      if (key == 'H')
        {
          if (no_items < 1)
            {
              printf("--------------------- \n");
              printf("OBS! No items exist in database.\n");
            } else
            {
              list_db(items, no_items);
            }
        }
      if (key == 'A')
        {
          printf("--------------------- \n");
          printf("Program Quit!\n");
          printf("--------------------- \n");
          break;
        } */    
    }
}
