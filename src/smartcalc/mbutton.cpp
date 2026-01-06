#include "mbutton.h"

MButton::MButton(const QString &text, QWidget *parent) : QToolButton(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setText(text);
}

QSize MButton::sizeHint() const {
  QSize size = QToolButton::sizeHint();
  size.rheight() += 30;
  size.rwidth() = qMax(size.width(), size.height());
  return size;
}
