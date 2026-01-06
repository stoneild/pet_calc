#include "plotwindow.h"

plotwindow::plotwindow(QWidget *parent)
    : QMainWindow{parent}, customPlot(new QCustomPlot(this)) {
  setCentralWidget(customPlot);
  setWindowTitle("Function Plot");
  showMaximized();
}

void plotwindow::plotFunction(QVector<double> &x, QVector<double> &y) {
  customPlot->clearGraphs();
  customPlot->addGraph();
  customPlot->graph(0)->setData(x, y);
  customPlot->xAxis->setLabel("x");
  customPlot->yAxis->setLabel("f(x)");

  customPlot->xAxis->setRange(*std::min_element(x.begin(), x.end()),
                              *std::max_element(x.begin(), x.end()));
  customPlot->yAxis->rescale();
  customPlot->replot();
}
