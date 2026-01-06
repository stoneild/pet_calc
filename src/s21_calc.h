#ifndef S21_CALC_H
#define S21_CALC_H

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_LEN 255

#define CODE_OK 0
#define CODE_PARSE_ERR 1
#define CODE_CALC_ERR 2
#define CODE_MEM_ERR 3

#define MOD 320
#define COS 325
#define SIN 330
#define TAN 323
#define ACOS 307
#define ASIN 315
#define ATAN 310
#define SQRT 342
#define LN 218
#define LOG 322
#define NEG 145
#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
#define POS 143
#define POW '^'

struct stack {
  uint_fast16_t weight;
  double value;
  struct stack *next;
};

typedef struct func_map {
  const char *name;
  double code;
  uint_fast16_t weight;
} func_map;

typedef struct operation_map {
  double code;
  int_fast8_t (*function)(struct stack **calc_stack);
} operation_map;

typedef struct credit_result_struct {
  uint_fast32_t months;
  double *monthly_payments;
  double total_payment;
  double overpayment;
} credit_result_struct;

typedef enum { CREDIT_ANNUITY, CREDIT_DIFFERENTIATED } credit_type_enum;

typedef struct deposit_result_struct {
  double accrued_interest;
  double tax_amount;
  double final_amount;
} deposit_result_struct;

/**
 * @brief Performs addition of two numbers
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_add(struct stack **calc_stack);

/**
 * @brief Performs subtraction of two numbers
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_sub(struct stack **calc_stack);

/**
 * @brief Performs multiplication of two numbers
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_mul(struct stack **calc_stack);

/**
 * @brief Performs division of two numbers
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_div(struct stack **calc_stack);

/**
 * @brief Calculates the remainder of division
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_mod(struct stack **calc_stack);

/**
 * @brief Raises one number to the power of another
 * @param calc_stack Pointer to the stack containing operands
 * @return Error code or CODE_OK
 */
int_fast8_t s21_pow(struct stack **calc_stack);

/**
 * @brief Applies unary minus to the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_neg(struct stack **calc_stack);

/**
 * @brief Calculates the cosine of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_cos(struct stack **calc_stack);

/**
 * @brief Calculates the sine of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_sin(struct stack **calc_stack);

/**
 * @brief Calculates the tangent of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_tan(struct stack **calc_stack);

/**
 * @brief Calculates the arccosine of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_acos(struct stack **calc_stack);

/**
 * @brief Calculates the arcsine of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_asin(struct stack **calc_stack);

/**
 * @brief Calculates the arctangent of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_atan(struct stack **calc_stack);

/**
 * @brief Calculates the square root of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_sqrt(struct stack **calc_stack);

/**
 * @brief Calculates the natural logarithm of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_ln(struct stack **calc_stack);

/**
 * @brief Calculates the base-10 logarithm of the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_log(struct stack **calc_stack);

/**
 * @brief Applies unary plus to the top value
 * @param calc_stack Pointer to the stack containing the operand
 * @return Error code or CODE_OK
 */
int_fast8_t s21_pos(struct stack **calc_stack);

/**
 * @brief Removes a lexeme from the stack and returns it
 * @param head_stack Pointer to the top of the stack
 * @return Pointer to the removed lexeme
 */
struct stack *pop_lexeme(
    struct stack **head_stack);  // Doesn't NULL the lexeme->next value

/**
 * @brief Frees the memory occupied by a lexeme
 * @param lexeme Pointer to the lexeme
 * @return CODE_OK or CODE_MEM_ERR if lexeme is NULL
 */
int_fast8_t destroy_lexeme(
    struct stack *lexeme);  // Returns CODE_MEM_ERR if lexeme == NULL

/**
 * @brief Pushes a prepared lexeme onto the stack
 * @param lexeme Pointer to the lexeme to push
 * @param head_stack Pointer to the top of the stack
 */
void push_lexeme(
    struct stack *lexeme,
    struct stack **head_stack);  // If there is no lexeme, nothing is done.

/**
 * @brief Creates and pushes a new lexeme onto the stack
 * @param head Pointer to the top of the stack
 * @param weight Operator priority
 * @param value Lexeme value (number or operation code)
 * @return CODE_OK or CODE_MEM_ERR
 */
int_fast8_t push_new_lexeme(struct stack **head, uint_fast16_t weight,
                            double value);

/**
 * @brief Checks the length of the expression
 * @param expr Expression string
 * @return 0 if valid, otherwise an error code
 */
int_fast8_t chk_len_expr(const char *expr);

/**
 * @brief Processes an operation and pushes it into the appropriate stack
 * @param buf_stack Pointer to the buffer stack
 * @param res_stack Pointer to the result stack
 * @param weight Operator priority
 * @param operation Operation code
 * @return CODE_OK or CODE_MEM_ERR
 */
int_fast8_t push_new_operation_lexeme(struct stack **buf_stack,
                                      struct stack **res_stack,
                                      uint_fast16_t weight, double operation);

/**
 * @brief Processes an alphabetical lexeme (e.g., sin, cos, log)
 * @param str Pointer to the current position in the expression string
 * @param buf_stack Pointer to the buffer stack
 * @param res_stack Pointer to the result stack
 * @return CODE_OK or CODE_PARSE_ERR
 */
int_fast8_t push_new_aplpha_lexeme(char **str, struct stack **buf_stack,
                                   struct stack **res_stack);

/**
 * @brief Processes a closing parenthesis
 * @param buf_stack Pointer to the buffer stack
 * @param res_stack Pointer to the result stack
 * @return CODE_OK or CODE_PARSE_ERR
 */
int_fast8_t proc_closing_bracket(struct stack **buf_stack,
                                 struct stack **res_stack);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Frees memory allocated for a stack structure
 * @param stack Pointer to the pointer of the stack to be freed.
 */
void free_stack(struct stack **stack);

/**
 * @brief Converts an expression to Reverse Polish Notation (RPN)
 * @param expr Input expression string
 * @param res_stack Pointer to the resulting RPN stack
 * @return Error code or CODE_OK
 */
int_fast8_t s21_get_rpn_stack(char *expr, struct stack **res_stack);

/**
 * @brief Evaluates the expression represented as an RPN stack
 * @param rpn_stack Stack in RPN format
 * @param argument Value of variable x (can be NULL)
 * @param result Pointer to store the result
 * @return Error code or CODE_OK
 */
int_fast8_t s21_calc_rpn_stack(struct stack *rpn_stack, double *argument,
                               double *result);

/**
 * @brief Main function to evaluate a mathematical expression
 * @param expr Expression string
 * @param result Pointer to store the result
 * @return Error code or CODE_OK
 */
int_fast8_t s21_calc_expr(char *expr, double *result);

/**
 * @brief Calculates credit payments and total amounts for a loan
 * @param amount Initial loan amount
 * @param months Loan term in months
 * @param rate Annual interest rate
 * @param type Type of credit (annuity or differentiated)
 * @return Pointer to the structure containing credit results
 */
credit_result_struct *calc_credit(double amoumt, uint_fast32_t months,
                                  double rate, credit_type_enum type);

/**
 * @brief Frees memory allocated for a credit result structure
 * @param result Pointer to the credit result structure to be freed
 */
void free_credit_result(credit_result_struct *result);

/**
 * @brief Calculates deposit income, taxes, and final amounts for a deposit
 * account
 * @param amount Initial deposit amount
 * @param opening_month Opening month of the deposit (1-12)
 * @param months Deposit term in months
 * @param interest_rate Annual interest rate
 * @param tax_rate Tax rate for interest income
 * @param payment_periodicity String defining the periodicity of interest
 * payments (e.g., "1", "2")
 * @param is_capitalization Flag indicating whether interest is capitalized (1)
 * or not (0)
 * @param transaction_months Array of months in which additional transactions
 * occur
 * @param transactions_amount Array of amounts for each additional transaction
 * @param transaction_cnt Number of additional transactions
 * @return Pointer to the structure containing deposit results
 */
deposit_result_struct *calc_deposit(double amoumt, uint_fast32_t opening_month,
                                    uint_fast32_t months, double interest_rate,
                                    double tax_rate, char *payment_periodicity,
                                    int_fast8_t is_capitalization,
                                    uint_fast32_t *transaction_months,
                                    double *transactions_amount,
                                    size_t transaction_cnt);

#ifdef __cplusplus
}
#endif

#endif  // S21_CALC_H
