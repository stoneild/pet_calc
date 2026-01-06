#include "./s21_calc.h"

deposit_result_struct *calc_deposit(double amount, uint_fast32_t opening_month,
                                    uint_fast32_t months, double interest_rate,
                                    double tax_rate, char *payment_periodicity,
                                    int_fast8_t is_capitalization,
                                    uint_fast32_t *transaction_months,
                                    double *transactions_amount,
                                    size_t transaction_cnt) {
  if (!months || !payment_periodicity) return NULL;
  deposit_result_struct *result = malloc(sizeof(deposit_result_struct));
  if (!result) return NULL;
  result->accrued_interest = 0.;
  result->tax_amount = 0.;
  result->final_amount = amount;
  double interest_buff = 0.;
  uint_fast32_t cap_period = atoi(payment_periodicity);
  uint_fast32_t prev_year = 0;
  double current_year_interest = 0.;
  size_t transaction_id = 0;
  for (uint_fast32_t i = 0; i < months; ++i) {
    uint_fast32_t current_year = (opening_month + i - 1) / 12;
    // Calc tax per last/prev year
    if (current_year != prev_year) {
      double taxable =
          current_year_interest - (210000);  // 210000 = 1000000.0 * 0.21
      if (taxable > 0.) result->tax_amount += taxable * (tax_rate / 100.0);
      current_year_interest = 0.;
      prev_year = current_year;
    }
    // Calc rate interest per month
    double monthly_interest =
        result->final_amount * (interest_rate / 100.0) / 12.0;
    result->accrued_interest += monthly_interest;
    current_year_interest += monthly_interest;
    if (is_capitalization) {
      interest_buff += monthly_interest;
      // Calc capitalization
      if ((cap_period > 0 && (i + 1) % cap_period == 0) || (i + 1 == months)) {
        result->final_amount += interest_buff;
        interest_buff = 0.;
      }
    }
    // Calc transactions
    while (transaction_id < transaction_cnt &&
           transaction_months[transaction_id] == i + 1) {
      result->final_amount += transactions_amount[transaction_id];
      ++transaction_id;
    }
  }
  if (!is_capitalization) result->final_amount += result->accrued_interest;
  // Calc tax per last year
  double final_taxable = current_year_interest - (1000000.0 * 0.21);
  if (final_taxable > 0.0) {
    result->tax_amount += final_taxable * (tax_rate / 100.0);
  }
  return result;
}
