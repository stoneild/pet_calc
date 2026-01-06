#ifndef SMARTCALC_H
#define SMARTCALC_H

#include <mbutton.h>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

#include "plotwindow.h"

#define FONT_SIZE 16
#define MAX_EXPR_SIZE 255
#define MAX_EXPR_FONT_SIZE 42
#define MIN_EXPR_FONT_SIZE 8

class SmartCalc : public QMainWindow {
  Q_OBJECT

 public:
  SmartCalc();
 private slots:
  void editModeBtnClicked(bool checked);
  void lexBtnClicked();
  void backspaceClicked();
  void clearAllClicked();
  void calcExpressionClicked();
  void expressionTextChanged();
  void plotFunctionClicked();
  void switchToSmartMode();
  void switchToCreditMode();
  void switchToDepositMode();
  void calcCreditClicked();
  void addDepositTransactionClicked();
  void removeDepositTransactionClicked();
  void calcDepositClicked();

 private:
  MButton *createButton(const QString &text, const char *member);
  // SmartCalc
  QWidget *m_central;
  QGridLayout *m_layout;
  QLabel *m_expression_info;
  QTextEdit *m_expression;
  QLabel *m_sign_eq;
  QLineEdit *m_result;
  QPushButton *m_toggle_edit_mode;
  MButton *m_left_bracket;
  MButton *m_right_bracket;
  MButton *m_all_clear;
  MButton *m_backspace;
  MButton *m_modulus;
  MButton *m_division;
  MButton *m_power;
  MButton *m_square_root;
  MButton *m_lex_buttons[10];
  MButton *m_multiplication;
  MButton *m_common_logarithm;
  MButton *m_natural_logarithm;
  MButton *m_subtraction;
  MButton *m_sine;
  MButton *m_arc_sine;
  MButton *m_addition;
  MButton *m_cosine;
  MButton *m_arc_cosine;
  MButton *m_point;
  MButton *m_argument;
  MButton *m_calc;
  MButton *m_tangent;
  MButton *m_arc_tangent;
  void adjustFontToFit(QTextEdit *editor, int maxLength = 255);

  MButton *m_plot_function;
  QLabel *m_step_label;
  QLabel *m_min_arg_label;
  QLabel *m_max_arg_label;
  QDoubleSpinBox *m_min_arg_value;
  QDoubleSpinBox *m_step_arg_value;
  QDoubleSpinBox *m_max_arg_value;
  plotwindow *m_plot_window = nullptr;

  // Common
  QDoubleValidator *monetary_validator;

  // CreditCalc
  QWidget *credit_central;
  QGridLayout *credit_layout;
  QLabel *credit_amount;
  QLineEdit *credit_amount_value;
  QLabel *credit_term;
  QDoubleSpinBox *credit_term_value;
  QLabel *credit_interest_rate;
  QDoubleSpinBox *credit_interest_rate_value;
  QLabel *credit_type;
  QComboBox *credit_type_value;
  MButton *credit_calc;
  QTextEdit *credit_result;

  // DepositCalc
  QWidget *deposit_central;
  QGridLayout *deposit_layout;

  QStackedWidget *stackedWidget;
  QLabel *deposit_amount;
  QLineEdit *deposit_amount_value;
  QLabel *deposit_opening_month;
  QComboBox *deposit_opening_month_value;
  QLabel *deposit_term;
  QDoubleSpinBox *deposit_term_value;
  QLabel *deposit_interest_rate;
  QDoubleSpinBox *deposit_interest_rate_value;
  QLabel *deposit_tax_rate;
  QDoubleSpinBox *deposit_tax_rate_value;
  QLabel *deposit_is_capitalization;
  QCheckBox *deposit_is_capitalization_value;
  QLabel *deposit_payments_periodicity;
  QComboBox *deposit_payments_periodicity_value;
  QLabel *deposit_transactions;
  QTableWidget *deposit_transaction_values;
  MButton *deposit_add_transaction;
  MButton *deposit_remove_transaction;
  MButton *deposit_calc;
  QTextEdit *deposit_result;
};

#endif  // SMARTCALC_H
