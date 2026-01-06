#include "./s21_calc.h"

int_fast8_t destroy_lexeme(struct stack *lexeme) {
  int_fast8_t status = CODE_OK;
  if (lexeme) {
    free(lexeme);
  } else {
    status = CODE_MEM_ERR;
  }
  return status;
}

int_fast8_t push_new_lexeme(struct stack **head, uint_fast16_t weight,
                            double value) {
  int_fast8_t status = CODE_OK;
  struct stack *new_lexeme = (struct stack *)malloc(sizeof(struct stack));
  if (NULL == new_lexeme) {
    status = CODE_MEM_ERR;
  } else {
    new_lexeme->weight = weight;
    new_lexeme->value = value;
    new_lexeme->next = *head;
    *head = new_lexeme;
  }
  return status;
}

void push_lexeme(struct stack *lexeme, struct stack **head_stack) {
  if (lexeme) {
    if (*head_stack) {
      lexeme->next = *head_stack;
      *head_stack = lexeme;
    } else {
      lexeme->next = NULL;
      *head_stack = lexeme;
    }
  }
}

struct stack *pop_lexeme(struct stack **head_stack) {
  struct stack *ptr = *head_stack;
  *head_stack = ptr->next;
  return ptr;
}
