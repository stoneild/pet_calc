#include "./s21_calc.h"

credit_result_struct *calc_credit(double amount, uint_fast32_t months,
                                  double rate, credit_type_enum type) {
  if (!months) return NULL;
  credit_result_struct *result = malloc(sizeof(credit_result_struct));
  if (!result) return NULL;
  result->months = months;
  result->monthly_payments = malloc(sizeof(result->monthly_payments) * months);
  result->total_payment = 0;
  result->overpayment = 0;
  if (!result->monthly_payments) {
    free(result);
    return NULL;
  }
  double monthly_rate = rate / 100. / 12.;
  if (type == CREDIT_ANNUITY) {
    double annuity_coeff = (monthly_rate * pow(1 + monthly_rate, months)) /
                           (pow(1 + monthly_rate, months) - 1);
    double monthly_payment = amount * annuity_coeff;
    for (uint_fast32_t i = 0; i < months; ++i) {
      result->monthly_payments[i] = monthly_payment;
      result->total_payment += monthly_payment;
    }
  } else {
    for (uint_fast32_t i = 0; i < months; ++i) {
      double fix_part = amount / months;
      double tax_part = (amount - fix_part * i) * monthly_rate;
      double payment = fix_part + tax_part;
      result->monthly_payments[i] = payment;
      result->total_payment += payment;
    }
  }
  result->overpayment = result->total_payment - amount;
  return result;
}

void free_credit_result(credit_result_struct *result) {
  if (result) {
    free(result->monthly_payments);
    free(result);
  }
}
