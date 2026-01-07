#include "./s21_calc.h"

int_fast8_t s21_calc_expr(char *expr, double *result) {
  if (!expr || chk_len_expr(expr) || !result) return CODE_PARSE_ERR;
  int_fast8_t status = CODE_OK;
  struct stack *res_stack = NULL;
  status = s21_get_rpn_stack(expr, &res_stack);
  if (!status) status = s21_calc_rpn_stack(res_stack, NULL, result);
  free_stack(&res_stack);
  return status;
}

int_fast8_t s21_get_rpn_stack(char *expr, struct stack **res_stack) {
  int_fast8_t status = CODE_OK;
  uint_fast16_t is_prev_operation = 1;  // unar operator flag
  struct stack *buf_stack = NULL;
  const func_map operation_lexeme_map[] = {
      {"+", ADD, 80}, {"-", SUB, 80}, {"*", MUL, 70}, {"/", DIV, 70},
      {"^", POW, 60}, {"+", POS, 51}, {"-", NEG, 51}, {NULL, -1, 0}};
  while (*expr && !status) {
    if (isspace(*expr)) {
      ++expr;
    } else if (is_prev_operation && (isdigit(*expr) || '.' == *expr)) {
      double value = strtod(expr, &expr);
      push_new_lexeme(&buf_stack, 10, value);
      is_prev_operation = 0;
    } else if (is_prev_operation && 'x' == *expr) {
      push_new_lexeme(&buf_stack, 0, 0);
      ++expr;
      is_prev_operation = 0;
    } else if (isalpha(*expr)) {
      status = push_new_alpha_lexeme(&expr, &buf_stack, res_stack);
      is_prev_operation = 1;
    } else if ('(' == *expr) {
      push_new_lexeme(res_stack, 101, '(');
      ++expr;
      is_prev_operation = 1;
    } else if (')' == *expr) {
      status = proc_closing_bracket(&buf_stack, res_stack);
      ++expr;
      is_prev_operation = 0;
    } else {
      size_t i = 0;
      while (operation_lexeme_map[i].name &&
             !(operation_lexeme_map[i].name[0] == *expr &&
               (operation_lexeme_map[i].weight & 1) == is_prev_operation))
        ++i;
      if (operation_lexeme_map[i].name) {
        status = push_new_operation_lexeme(&buf_stack, res_stack,
                                           operation_lexeme_map[i].weight,
                                           operation_lexeme_map[i].code);
        ++expr;
        is_prev_operation = 1;
      } else {
        status = CODE_PARSE_ERR;
      }
    }
  }
  if (is_prev_operation)
    status = CODE_PARSE_ERR;
  else {
    struct stack *ptr = *res_stack;
    while (!status && ptr) {
      if (ptr->weight == 101) status = CODE_PARSE_ERR;
      ptr = ptr->next;
    }
    if (!status) {
      while (buf_stack) push_lexeme(pop_lexeme(&buf_stack), res_stack);
    }
  }
  free_stack(&buf_stack);
  return status;
}

int_fast8_t chk_len_expr(const char *expr) {
  uint_fast16_t i;
  for (i = 0; i <= 255 && expr[i] != '\0'; ++i);
  return i > 255;
}

int_fast8_t push_new_alpha_lexeme(char **expr, struct stack **buf_stack,
                                  struct stack **res_stack) {
  int_fast8_t status = CODE_OK;
  char alpha_lexeme[MAX_EXPR_LEN];
  char *ptr = alpha_lexeme;
  while (**expr && isalpha(**expr) && (ptr - alpha_lexeme) < (MAX_EXPR_LEN - 1))
    *ptr++ = *(*expr)++;
  *ptr = '\0';
  size_t i = 0;
  const func_map alpha_lexeme_map[] = {
      {"mod", MOD, 70},   {"cos", COS, 41},   {"sin", SIN, 41},
      {"tan", TAN, 41},   {"acos", ACOS, 41}, {"asin", ASIN, 41},
      {"atan", ATAN, 41}, {"sqrt", SQRT, 41}, {"ln", LN, 41},
      {"log", LOG, 41},   {"x", 0, 0},        {NULL, -1, 0}};
  while (alpha_lexeme_map[i].name &&
         strcmp(alpha_lexeme, alpha_lexeme_map[i].name))
    ++i;
  if (alpha_lexeme_map[i].name) {
    status = push_new_operation_lexeme(buf_stack, res_stack,
                                       alpha_lexeme_map[i].weight,
                                       alpha_lexeme_map[i].code);
  } else {
    status = CODE_PARSE_ERR;
  }
  return status;
}

int_fast8_t push_new_operation_lexeme(struct stack **buf_stack,
                                      struct stack **res_stack,
                                      uint_fast16_t weight, double operation) {
  int_fast8_t status = CODE_OK;
  if ((*res_stack) && operation == (*res_stack)->value) {
    status = CODE_PARSE_ERR;
  } else if (60 == weight || 51 == weight) {
    while (*res_stack && weight > (*res_stack)->weight) {
      push_lexeme(pop_lexeme(res_stack), buf_stack);
    }
  } else {
    while (*res_stack && weight >= (*res_stack)->weight) {
      push_lexeme(pop_lexeme(res_stack), buf_stack);
    }
  }
  return status || push_new_lexeme(res_stack, weight, operation);
}

int_fast8_t proc_closing_bracket(struct stack **buf_stack,
                                 struct stack **res_stack) {
  if (!*res_stack) return CODE_PARSE_ERR;
  int_fast8_t status = CODE_OK;
  while (*res_stack && (*res_stack)->next && '(' != (*res_stack)->value) {
    push_lexeme(pop_lexeme(res_stack), buf_stack);
  }
  if (*res_stack && '(' == (*res_stack)->value) {
    status = destroy_lexeme(pop_lexeme(res_stack));
  } else {
    status = CODE_PARSE_ERR;
  }
  return status;
}

int_fast8_t s21_calc_rpn_stack(struct stack *rpn_stack, double *argument,
                               double *result) {
  if (!rpn_stack) return CODE_PARSE_ERR;
  int_fast8_t status = CODE_OK;
  struct stack *ptr;
  ptr = rpn_stack;
  if (argument) {
    while (ptr) {
      if (!ptr->weight) {
        ptr->value = *argument;
      }
      ptr = ptr->next;
    }
  }
  ptr = NULL;
  const operation_map op[] = {
      {NEG, s21_neg},   {COS, s21_cos},   {SIN, s21_sin},   {TAN, s21_tan},
      {ACOS, s21_acos}, {ASIN, s21_asin}, {ATAN, s21_atan}, {SQRT, s21_sqrt},
      {LN, s21_ln},     {LOG, s21_log},   {POS, s21_pos},   {ADD, s21_add},
      {SUB, s21_sub},   {MUL, s21_mul},   {DIV, s21_div},   {MOD, s21_mod},
      {POW, s21_pow},   {0, NULL}};
  for (; !status && rpn_stack; rpn_stack = rpn_stack->next) {
    if (rpn_stack->weight <= 10) {
      push_new_lexeme(&ptr, rpn_stack->weight, rpn_stack->value);
    } else {
      size_t i = 0;
      for (; op[i].function && !(rpn_stack->value == op[i].code); ++i);
      status = op[i].function(&ptr);
    }
  }
  *result = ptr->value;
  if (!status) status = (NULL != ptr->next);
  free_stack(&ptr);
  return status;
}

void free_stack(struct stack **stack) {
  while (*stack) destroy_lexeme(pop_lexeme(stack));
}
