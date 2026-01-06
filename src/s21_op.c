#include "./s21_calc.h"

int_fast8_t s21_add(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value += ptr->value;
  return destroy_lexeme(ptr);
}

int_fast8_t s21_sub(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value -= ptr->value;
  return destroy_lexeme(ptr);
}

int_fast8_t s21_mul(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value *= ptr->value;
  return destroy_lexeme(ptr);
}

int_fast8_t s21_div(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value /= ptr->value;
  return destroy_lexeme(ptr);
}

int_fast8_t s21_mod(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value = fmod((*calc_stack)->value, ptr->value);
  return destroy_lexeme(ptr);
}

int_fast8_t s21_pow(struct stack **calc_stack) {
  if (!(*calc_stack)->next) return CODE_CALC_ERR;
  struct stack *ptr = *calc_stack;
  *calc_stack = (*calc_stack)->next;
  (*calc_stack)->value = pow((*calc_stack)->value, ptr->value);
  return destroy_lexeme(ptr);
}

int_fast8_t s21_neg(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = -(*calc_stack)->value;
  return CODE_OK;
}

int_fast8_t s21_cos(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = cos((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_sin(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = sin((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_tan(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = tan((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_acos(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = acos((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_asin(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = asin((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_atan(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = atan((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_sqrt(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = sqrt((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_ln(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = log((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_log(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  (*calc_stack)->value = log10((*calc_stack)->value);
  return CODE_OK;
}

int_fast8_t s21_pos(struct stack **calc_stack) {
  if (!(*calc_stack)) return CODE_CALC_ERR;
  return CODE_OK;
}
