#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"

class plotwindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit plotwindow(QWidget *parent = nullptr);
  void plotFunction(QVector<double> &x, QVector<double> &y);

 private:
  QCustomPlot *customPlot;
 signals:
};

#endif  // PLOTWINDOW_H
