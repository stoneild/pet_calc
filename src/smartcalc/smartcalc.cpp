#include "smartcalc.h"

#include <QFontDatabase>

#include "plotwindow.h"
#include "s21_calc.h"

SmartCalc::SmartCalc() {
  // Menu
  QMenu *modeMenu = menuBar()->addMenu("Mode");
  QAction *smartModeAction = modeMenu->addAction("SmartCalc");
  QAction *creditModeAction = modeMenu->addAction("CreditCalc");
  QAction *depositModeAction = modeMenu->addAction("DepositCalc");
  connect(smartModeAction, &QAction::triggered, this,
          &SmartCalc::switchToSmartMode);
  connect(creditModeAction, &QAction::triggered, this,
          &SmartCalc::switchToCreditMode);
  connect(depositModeAction, &QAction::triggered, this,
          &SmartCalc::switchToDepositMode);

  // SmartCalc
  m_central = new QWidget(this);
  m_layout = new QGridLayout(m_central);
  m_layout->setSizeConstraint(QLayout::SetFixedSize);
  m_expression = new QTextEdit();
  QFont font =
      QFontDatabase::systemFont(QFontDatabase::FixedFont);  // Font settings
  font.setPointSize(FONT_SIZE);
  QFont font_expr = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  font_expr.setPointSize(MAX_EXPR_FONT_SIZE);
  m_expression->setReadOnly(true);
  m_expression->setFixedHeight(52);
  m_expression->setFont(font_expr);
  m_expression->document()->setDocumentMargin(0);
  m_expression->setContentsMargins(0, 0, 0, 0);
  m_expression->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_expression->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  connect(m_expression, &QTextEdit::textChanged, this,
          &SmartCalc::expressionTextChanged);
  m_expression_info = new QLabel();
  m_expression_info->setAlignment(Qt::AlignCenter);
  m_expression_info->setText(QString("Used %1 of %2 characters")
                                 .arg(m_expression->toPlainText().length())
                                 .arg(MAX_EXPR_SIZE));
  m_sign_eq = new QLabel();
  m_sign_eq->setText("=");
  m_sign_eq->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
  m_sign_eq->setFont(font);
  //    m_sign_eq->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_result = new QLineEdit();
  m_result->setReadOnly(true);
  m_result->setFixedHeight(52);
  m_result->setAlignment(Qt::AlignRight);
  m_result->setText("Enter expression");
  m_result->setFont(font);
  m_toggle_edit_mode = new QPushButton("Edit Mode");
  m_toggle_edit_mode->setCheckable(true);
  m_toggle_edit_mode->setChecked(!m_expression->isReadOnly());
  connect(m_toggle_edit_mode, &QPushButton::toggled, this,
          &SmartCalc::editModeBtnClicked);
  m_left_bracket = createButton("(", SLOT(lexBtnClicked()));
  m_right_bracket = createButton(")", SLOT(lexBtnClicked()));
  m_all_clear = createButton("AC", SLOT(clearAllClicked()));
  m_backspace = createButton(QChar(0x0000232b), SLOT(backspaceClicked()));
  m_modulus = createButton("mod", SLOT(lexBtnClicked()));
  m_division = createButton("/", SLOT(lexBtnClicked()));
  m_power = createButton("^", SLOT(lexBtnClicked()));
  m_square_root = createButton("sqrt(", SLOT(lexBtnClicked()));
  for (uint_fast8_t i = 0; i < 10; ++i) {
    m_lex_buttons[i] = createButton(QString::number(i), SLOT(lexBtnClicked()));
  }
  m_multiplication = createButton("*", SLOT(lexBtnClicked()));
  m_common_logarithm = createButton("log(", SLOT(lexBtnClicked()));
  m_natural_logarithm = createButton("ln(", SLOT(lexBtnClicked()));
  m_subtraction = createButton("-", SLOT(lexBtnClicked()));
  m_sine = createButton("sin(", SLOT(lexBtnClicked()));
  m_arc_sine = createButton("asin(", SLOT(lexBtnClicked()));
  m_addition = createButton("+", SLOT(lexBtnClicked()));
  m_cosine = createButton("cos(", SLOT(lexBtnClicked()));
  m_arc_cosine = createButton("acos(", SLOT(lexBtnClicked()));
  m_point = createButton(".", SLOT(lexBtnClicked()));  // переделать логику?
  m_argument = createButton("x", SLOT(lexBtnClicked()));
  m_calc = createButton("=", SLOT(calcExpressionClicked()));
  m_calc->setEnabled(false);
  m_tangent = createButton("tan(", SLOT(lexBtnClicked()));
  m_arc_tangent = createButton("atan(", SLOT(lexBtnClicked()));

  m_plot_function = createButton("Plot function", SLOT(plotFunctionClicked()));
  m_plot_function->setEnabled(false);
  m_min_arg_label = new QLabel();
  m_min_arg_label->setText("Min x:");
  m_min_arg_label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
  m_min_arg_label->setEnabled(false);
  m_min_arg_value = new QDoubleSpinBox();
  m_min_arg_value->setRange(-1000000, 999999.9);
  m_min_arg_value->setDecimals(1);
  m_min_arg_value->setValue(-1000000);
  m_min_arg_value->setEnabled(false);
  m_step_label = new QLabel();
  m_step_label->setText("Step:");
  m_step_label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
  m_step_label->setEnabled(false);
  m_step_arg_value = new QDoubleSpinBox();
  m_step_arg_value->setRange(0.1, 1000000);
  m_step_arg_value->setDecimals(1);
  m_step_arg_value->setSingleStep(0.1);
  m_step_arg_value->setValue(0.5);
  m_step_arg_value->setEnabled(false);
  m_max_arg_label = new QLabel();
  m_max_arg_label->setText("Max x:");
  m_max_arg_label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
  m_max_arg_label->setEnabled(false);
  m_max_arg_value = new QDoubleSpinBox();
  m_max_arg_value->setRange(-999999.9, 1000000);
  m_max_arg_value->setDecimals(1);
  m_max_arg_value->setValue(1000000);
  m_max_arg_value->setEnabled(false);

  m_layout->addWidget(m_expression_info, 0, 0, 1, 6);
  m_layout->addWidget(m_expression, 1, 0, 1, 6);
  m_layout->addWidget(m_sign_eq, 2, 0, 1, 1);
  m_layout->addWidget(m_result, 2, 1, 1, 5);
  m_layout->addWidget(m_toggle_edit_mode, 3, 0, 1, 4);
  m_layout->addWidget(m_left_bracket, 3, 4, 1, 1);
  m_layout->addWidget(m_right_bracket, 3, 5, 1, 1);
  m_layout->addWidget(m_all_clear, 4, 0, 1, 1);
  m_layout->addWidget(m_backspace, 4, 1, 1, 1);
  m_layout->addWidget(m_modulus, 4, 2, 1, 1);
  m_layout->addWidget(m_division, 4, 3, 1, 1);
  m_layout->addWidget(m_power, 4, 4, 1, 1);
  m_layout->addWidget(m_square_root, 4, 5, 1, 1);
  for (uint_fast8_t i = 1; i < 10; ++i) {
    uint_fast8_t row = ((9 - i) / 3) + 5;
    uint_fast8_t column = ((i - 1) % 3);
    m_layout->addWidget(m_lex_buttons[i], row, column, 1, 1);
  }
  m_layout->addWidget(m_lex_buttons[0], 8, 0, 1, 1);
  m_layout->addWidget(m_multiplication, 5, 3, 1, 1);
  m_layout->addWidget(m_common_logarithm, 5, 4, 1, 1);
  m_layout->addWidget(m_natural_logarithm, 5, 5, 1, 1);
  m_layout->addWidget(m_subtraction, 6, 3, 1, 1);
  m_layout->addWidget(m_sine, 6, 4, 1, 1);
  m_layout->addWidget(m_arc_sine, 6, 5, 1, 1);
  m_layout->addWidget(m_addition, 7, 3, 1, 1);
  m_layout->addWidget(m_cosine, 7, 4, 1, 1);
  m_layout->addWidget(m_arc_cosine, 7, 5, 1, 1);
  m_layout->addWidget(m_point, 8, 1, 1, 1);
  m_layout->addWidget(m_argument, 8, 2, 1, 1);
  m_layout->addWidget(m_calc, 8, 3, 1, 1);
  m_layout->addWidget(m_tangent, 8, 4, 1, 1);
  m_layout->addWidget(m_arc_tangent, 8, 5, 1, 1);

  m_layout->addWidget(m_plot_function, 9, 0, 1, 3);
  m_layout->addWidget(m_min_arg_label, 9, 3, 1, 1);
  m_layout->addWidget(m_min_arg_value, 9, 4, 1, 2);
  m_layout->addWidget(m_step_label, 10, 0, 1, 1);
  m_layout->addWidget(m_step_arg_value, 10, 1, 1, 2);
  m_layout->addWidget(m_max_arg_label, 10, 3, 1, 1);
  m_layout->addWidget(m_max_arg_value, 10, 4, 1, 2);

  // Common
  monetary_validator = new QDoubleValidator(0.0, 1e18, 2, this);

  // CreditCalc
  credit_central = new QWidget(this);
  credit_layout = new QGridLayout(credit_central);
  credit_layout->setSizeConstraint(QLayout::SetFixedSize);
  credit_amount = new QLabel();
  credit_amount->setText("Total credit amount:");
  credit_amount_value = new QLineEdit();
  credit_amount_value->setValidator(monetary_validator);
  credit_amount_value->setText("100000");
  credit_term = new QLabel();
  credit_term->setText("Credit term (months):");
  credit_term_value = new QDoubleSpinBox();
  credit_term_value->setRange(1, 999999);
  credit_term_value->setDecimals(0);
  credit_term_value->setValue(12);
  credit_interest_rate = new QLabel();
  credit_interest_rate->setText("Credit interest rate:");
  credit_interest_rate_value = new QDoubleSpinBox();
  credit_interest_rate_value->setRange(0, 999999);
  credit_interest_rate_value->setDecimals(1);
  credit_interest_rate_value->setValue(21);
  credit_type = new QLabel();
  credit_type->setText("Credit type:");
  credit_type_value = new QComboBox();
  credit_type_value->addItem("Annuity");
  credit_type_value->addItem("Differentiated");
  credit_calc = createButton("Calc credit", SLOT(calcCreditClicked()));

  credit_result = new QTextEdit();
  credit_result->setReadOnly(true);
  credit_result->setFixedHeight(350);
  credit_result->setFont(font);
  credit_result->document()->setDocumentMargin(0);
  credit_result->setContentsMargins(0, 0, 0, 0);

  credit_layout->addWidget(credit_amount, 0, 0, 1, 6);
  credit_layout->addWidget(credit_amount_value, 1, 0, 1, 6);
  credit_layout->addWidget(credit_term, 2, 0, 1, 6);
  credit_layout->addWidget(credit_term_value, 3, 0, 1, 6);
  credit_layout->addWidget(credit_interest_rate, 4, 0, 1, 6);
  credit_layout->addWidget(credit_interest_rate_value, 5, 0, 1, 6);
  credit_layout->addWidget(credit_type, 6, 0, 1, 2);
  credit_layout->addWidget(credit_type_value, 6, 2, 1, 4);
  credit_layout->addWidget(credit_calc, 7, 0, 1, 6);
  credit_layout->addWidget(credit_result, 8, 0, 1, 6);

  // DepositCalc
  deposit_central = new QWidget(this);
  deposit_layout = new QGridLayout(deposit_central);
  deposit_layout->setSizeConstraint(QLayout::SetFixedSize);
  deposit_amount = new QLabel();
  deposit_amount->setText("Deposit amount:");
  deposit_amount_value = new QLineEdit();
  deposit_amount_value->setValidator(monetary_validator);
  deposit_amount_value->setText("100000");
  deposit_opening_month = new QLabel();
  deposit_opening_month->setText("Opening month:");
  deposit_opening_month_value = new QComboBox();
  QStringList months = {"January",   "February", "March",    "April",
                        "May",       "June",     "July",     "August",
                        "September", "October",  "November", "December"};
  deposit_opening_month_value->addItems(months);
  deposit_opening_month_value->setCurrentIndex(QDate::currentDate().month() %
                                               12);
  deposit_term = new QLabel();
  deposit_term->setText("Dposit term (months):");
  deposit_term_value = new QDoubleSpinBox();
  deposit_term_value->setRange(1, 999999);
  deposit_term_value->setDecimals(0);
  deposit_term_value->setValue(12);
  deposit_interest_rate = new QLabel();
  deposit_interest_rate->setText("Deposit interest rate:");
  deposit_interest_rate_value = new QDoubleSpinBox();
  deposit_interest_rate_value->setRange(0, 999999);
  deposit_interest_rate_value->setDecimals(1);
  deposit_interest_rate_value->setValue(21);
  deposit_tax_rate = new QLabel();
  deposit_tax_rate->setText("Tax reate:");
  deposit_tax_rate_value = new QDoubleSpinBox();
  deposit_tax_rate_value->setRange(0, 999999);
  deposit_tax_rate_value->setDecimals(1);
  deposit_tax_rate_value->setValue(13);
  deposit_payments_periodicity = new QLabel();
  deposit_payments_periodicity->setText("Periodicity of payments (months):");
  deposit_payments_periodicity_value = new QComboBox();
  QStringList periods = {"Upon closure", "1", "3", "6"};
  deposit_payments_periodicity_value->addItems(periods);
  deposit_transactions = new QLabel();
  deposit_transactions->setText("Replenishments and Withdrawals:");
  deposit_is_capitalization = new QLabel();
  deposit_is_capitalization->setText("Capitalization:");
  deposit_is_capitalization_value = new QCheckBox();
  deposit_is_capitalization_value->setChecked(false);
  deposit_transaction_values = new QTableWidget();
  deposit_transaction_values->setColumnCount(2);
  deposit_transaction_values->setHorizontalHeaderLabels(
      QStringList() << "Month" << "Transaction amount");
  deposit_transaction_values->horizontalHeader()->setStretchLastSection(true);
  deposit_transaction_values->setEditTriggers(
      QAbstractItemView::AllEditTriggers);
  deposit_transaction_values->setSelectionBehavior(
      QAbstractItemView::SelectRows);
  deposit_transaction_values->setSelectionMode(
      QAbstractItemView::SingleSelection);
  deposit_transaction_values->setFixedHeight(96);
  deposit_add_transaction =
      createButton("Add row", SLOT(addDepositTransactionClicked()));
  deposit_add_transaction->setFixedHeight(20);
  deposit_remove_transaction =
      createButton("Remove row", SLOT(removeDepositTransactionClicked()));
  deposit_remove_transaction->setFixedHeight(20);
  deposit_calc = createButton("Calc deposit", SLOT(calcDepositClicked()));

  deposit_result = new QTextEdit();
  deposit_result->setReadOnly(true);
  deposit_result->setFixedHeight(144);
  deposit_result->setFont(font);
  deposit_result->document()->setDocumentMargin(0);
  deposit_result->setContentsMargins(0, 0, 0, 0);

  deposit_layout->addWidget(deposit_amount, 0, 0, 1, 3);
  deposit_layout->addWidget(deposit_amount_value, 0, 3, 1, 3);
  deposit_layout->addWidget(deposit_opening_month, 2, 0, 1, 3);
  deposit_layout->addWidget(deposit_opening_month_value, 2, 3, 1, 3);
  deposit_layout->addWidget(deposit_term, 4, 0, 1, 3);
  deposit_layout->addWidget(deposit_term_value, 4, 3, 1, 3);
  deposit_layout->addWidget(deposit_interest_rate, 6, 0, 1, 3);
  deposit_layout->addWidget(deposit_interest_rate_value, 6, 3, 1, 3);
  deposit_layout->addWidget(deposit_tax_rate, 8, 0, 1, 3);
  deposit_layout->addWidget(deposit_tax_rate_value, 8, 3, 1, 3);
  deposit_layout->addWidget(deposit_payments_periodicity, 10, 0, 1, 6);
  deposit_layout->addWidget(deposit_payments_periodicity_value, 11, 0, 1, 6);
  deposit_layout->addWidget(deposit_is_capitalization, 12, 0, 1, 3);
  deposit_layout->addWidget(deposit_is_capitalization_value, 12, 3, 1, 3);
  deposit_layout->addWidget(deposit_transactions, 13, 0, 1, 6);
  deposit_layout->addWidget(deposit_transaction_values, 14, 0, 1, 6);
  deposit_layout->addWidget(deposit_add_transaction, 15, 0, 1, 3);
  deposit_layout->addWidget(deposit_remove_transaction, 15, 3, 1, 3);
  deposit_layout->addWidget(deposit_calc, 16, 0, 1, 6);
  deposit_layout->addWidget(deposit_result, 17, 0, 1, 6);

  // Set central QWidget
  setWindowTitle("SmartCalc");
  stackedWidget = new QStackedWidget(this);
  stackedWidget->addWidget(m_central);
  stackedWidget->addWidget(credit_central);
  stackedWidget->addWidget(deposit_central);

  stackedWidget->setCurrentWidget(m_central);
  setCentralWidget(stackedWidget);
}

void SmartCalc::editModeBtnClicked(bool checked) {
  m_expression->setReadOnly(!checked);
  m_toggle_edit_mode->setText(checked ? "Edit mode: ON" : "Edit mode: OFF");
  m_expression->moveCursor(QTextCursor::End);
}

void SmartCalc::lexBtnClicked() {
  m_expression->insertPlainText(((MButton *)sender())->text());
  if (m_calc->isEnabled()) m_result->clear();
}

void SmartCalc::backspaceClicked() {
  QTextCursor cursor = m_expression->textCursor();
  if (!cursor.atStart()) {
    cursor.deletePreviousChar();
    m_expression->setTextCursor(cursor);
  }
  if (m_calc->isEnabled()) m_result->clear();
}

void SmartCalc::clearAllClicked() { m_expression->clear(); }

void SmartCalc::calcExpressionClicked() {
  QString exprText = m_expression->toPlainText();  // Get expression
  QByteArray byteArray = exprText.toUtf8();        // Convert to C-style string
  char *exprCStr = byteArray.data();
  double result;
  int_fast8_t status = s21_calc_expr(exprCStr, &result);
  if (status == 0) {
    m_result->setText(QString::number(result, 'g', 15));
  } else {
    m_result->setText("Error");
  }
}

void SmartCalc::expressionTextChanged() {
  adjustFontToFit(m_expression, MAX_EXPR_SIZE);
  // Change the text lenth utilization
  m_expression_info->setText(QString("Used %1 of %2 characters")
                                 .arg(m_expression->toPlainText().length())
                                 .arg(MAX_EXPR_SIZE));
  // Проверка на наличие аргумента "x"
  const QString expr = m_expression->toPlainText();
  if (expr.contains('x', Qt::CaseInsensitive)) {
    m_result->setText("\"x\" detected - plot enable");
    m_calc->setEnabled(false);
    m_plot_function->setEnabled(true);
    m_min_arg_label->setEnabled(true);
    m_min_arg_value->setEnabled(true);
    m_step_label->setEnabled(true);
    m_step_arg_value->setEnabled(true);
    m_max_arg_label->setEnabled(true);
    m_max_arg_value->setEnabled(true);
  } else {
    bool isEmpty = m_expression->toPlainText().trimmed().isEmpty();
    m_calc->setEnabled(!isEmpty);
    if (isEmpty) m_result->setText("Enter expression");
    m_plot_function->setEnabled(false);
    m_min_arg_label->setEnabled(false);
    m_min_arg_value->setEnabled(false);
    m_step_label->setEnabled(false);
    m_step_arg_value->setEnabled(false);
    m_max_arg_label->setEnabled(false);
    m_max_arg_value->setEnabled(false);
  }
}

void SmartCalc::plotFunctionClicked() {
  QString expr = m_expression->toPlainText();
  double min = m_min_arg_value->value();
  double max = m_max_arg_value->value();
  double step = m_step_arg_value->value();
  if (min >= max || step <= 0) {
    return;
  }
  // Преобразование выражения в C-строку
  QByteArray exprBytes = expr.toLocal8Bit();
  char *c_expr = exprBytes.data();
  struct stack *rpn_stack = nullptr;
  int_fast8_t status = s21_get_rpn_stack(c_expr, &rpn_stack);
  // Check for the stack
  if (status != 0 || rpn_stack == nullptr) {
    m_result->setText("Parse Error: fix the expression");
  } else {
    // Calc points
    QVector<double> x, y;
    for (double val = min; val <= max; val += step) {
      double result = 0;
      if (s21_calc_rpn_stack(rpn_stack, &val, &result) == 0) {
        x.push_back(val);
        y.push_back(result);
      }
    }
    // Показать окно с графиком
    if (!m_plot_window) {
      m_plot_window = new plotwindow();
    }
    m_plot_window->plotFunction(x, y);
    m_plot_window->show();
    m_plot_window->raise();
    m_plot_window->activateWindow();
  }
  free_stack(&rpn_stack);
}

void SmartCalc::switchToSmartMode() {
  stackedWidget->setCurrentWidget(m_central);
  setWindowTitle("SmartCalc");
}

void SmartCalc::switchToCreditMode() {
  stackedWidget->setCurrentWidget(credit_central);
  setWindowTitle("CreditCalc");
}

void SmartCalc::switchToDepositMode() {
  stackedWidget->setCurrentWidget(deposit_central);
  setWindowTitle("DepositCalc");
}

void SmartCalc::calcCreditClicked() {
  double amount = credit_amount_value->text().toDouble();
  uint_fast32_t months = credit_term_value->value();
  double rate = credit_interest_rate_value->value();
  credit_type_enum type = (credit_type_value->currentText() == "Annuity")
                              ? CREDIT_ANNUITY
                              : CREDIT_DIFFERENTIATED;
  credit_result_struct *result = calc_credit(amount, months, rate, type);
  if (!result) {
    credit_result->setText("Calculate error");
  } else {
    QString report;
    report +=
        QString("Total payment: %1\n").arg(result->total_payment, 0, 'f', 2);
    report += QString("Overpayment: %1\n").arg(result->overpayment, 0, 'f', 2);
    report += QString("Monthly payments:\n");
    for (uint_fast32_t i = 0; i < result->months; ++i) {
      report += QString("%1. %2\n")
                    .arg(i + 1)
                    .arg(result->monthly_payments[i], 0, 'f', 2);
    }
    credit_result->setPlainText(report);
  }
  free_credit_result(result);
}

void SmartCalc::addDepositTransactionClicked() {
  QIntValidator *month_validator =
      new QIntValidator(1, std::numeric_limits<int>::max(), this);
  QDoubleValidator *monetary_validator =
      new QDoubleValidator(-1e18, 1e18, 2, this);
  uint_fast32_t row_cnt = deposit_transaction_values->rowCount();
  deposit_transaction_values->insertRow(row_cnt);
  QLineEdit *transaction_month = new QLineEdit();
  transaction_month->setValidator(month_validator);
  deposit_transaction_values->setCellWidget(row_cnt, 0, transaction_month);
  QLineEdit *transaction_amount = new QLineEdit();
  transaction_amount->setValidator(monetary_validator);
  deposit_transaction_values->setCellWidget(row_cnt, 1, transaction_amount);
}

void SmartCalc::removeDepositTransactionClicked() {
  uint_fast32_t row_cnt = deposit_transaction_values->rowCount();
  if (row_cnt > 0) deposit_transaction_values->removeRow(row_cnt - 1);
}

void SmartCalc::calcDepositClicked() {
  const uint_fast32_t months =
      static_cast<uint_fast32_t>(deposit_term_value->value());
  // Prepair transactions
  std::vector<std::pair<uint_fast32_t, double>> transactions;
  for (int i = 0; i < deposit_transaction_values->rowCount(); ++i) {
    QLineEdit *month_edit =
        qobject_cast<QLineEdit *>(deposit_transaction_values->cellWidget(i, 0));
    QLineEdit *amount_edit =
        qobject_cast<QLineEdit *>(deposit_transaction_values->cellWidget(i, 1));
    if (month_edit && amount_edit && !month_edit->text().isEmpty()) {
      uint_fast32_t month = month_edit->text().toUInt();
      if (month >= 1 && month <= months) {
        double transaction_amount = amount_edit->text().toDouble();
        transactions.emplace_back(month, transaction_amount);
      }
    }
  }
  std::sort(transactions.begin(), transactions.end(),
            [](const std::pair<uint_fast32_t, double> &a,
               const std::pair<uint_fast32_t, double> &b) {
              return a.first < b.first;
            });
  std::vector<uint_fast32_t> transaction_months;
  std::vector<double> transactions_amount;
  for (const std::pair<uint_fast32_t, double> &pair : transactions) {
    transaction_months.push_back(pair.first);
    transactions_amount.push_back(pair.second);
  }

  deposit_result_struct *result = calc_deposit(
      deposit_amount_value->text().toDouble(),
      deposit_opening_month_value->currentIndex() + 1, months,
      deposit_interest_rate_value->value(), deposit_tax_rate_value->value(),
      deposit_payments_periodicity_value->currentText().toUtf8().data(),
      deposit_is_capitalization_value->isChecked() ? 1 : 0,
      transaction_months.empty() ? nullptr : transaction_months.data(),
      transactions_amount.empty() ? nullptr : transactions_amount.data(),
      transaction_months.size());

  if (result) {
    deposit_result->setText(
        QString("Accrued interest:\n%1\nTax amount:\n%2\nDeposit amount by "
                "the end of the term:\n%3")
            .arg(result->accrued_interest, 0, 'f', 2)
            .arg(result->tax_amount, 0, 'f', 2)
            .arg(result->final_amount, 0, 'f', 2));
    free(result);
  } else {
    deposit_result->setText("Calculate error");
  }
}

MButton *SmartCalc::createButton(const QString &text, const char *member) {
  MButton *button = new MButton(text);
  connect(button, SIGNAL(clicked()), this, member);
  return button;
}

void SmartCalc::adjustFontToFit(QTextEdit *editor, int maxLength) {
  QString text = editor->toPlainText();
  if (text.isEmpty()) return;
  QTextCursor cursor = editor->textCursor();  // Save a cursor position
  // Set the expression max length
  if (text.length() > maxLength) {
    int pos = cursor.position();  // Save a cursor position
    text.truncate(maxLength);
    editor->blockSignals(true);
    editor->setPlainText(text);
    cursor.setPosition(std::min(pos, maxLength));
    editor->setTextCursor(cursor);
    editor->blockSignals(false);
  }
  // Font settings
  QFont font = editor->font();
  int widgetWidth = editor->viewport()->width() - 1;
  int fontSize = font.pointSize();
  int minFont = MIN_EXPR_FONT_SIZE;
  int maxFont = MAX_EXPR_FONT_SIZE;
  QFontMetrics fm(font);
  // Decrease font size
  while (
      QFontMetrics(QFont(font.family(), fontSize + 1)).horizontalAdvance(text) >
          widgetWidth &&
      fontSize > minFont) {
    font.setPointSize(--fontSize);
    fm = QFontMetrics(font);
  }
  // Increase font size
  while (fontSize + 1 < maxFont &&
         QFontMetrics(QFont(font.family(), fontSize + 1))
                 .horizontalAdvance(text) <= widgetWidth) {
    fontSize++;
    font.setPointSize(fontSize);
    fm = QFontMetrics(font);
  }
  editor->setFont(font);
  editor->setTextCursor(cursor);  // Restore the cursor position
}
