#ifndef MBUTTON_H
#define MBUTTON_H

#include <QToolButton>

class MButton : public QToolButton {
  Q_OBJECT
 public:
  explicit MButton(const QString &text, QWidget *parent = nullptr);

  QSize sizeHint() const override;
};

#endif  // MBUTTON_H
