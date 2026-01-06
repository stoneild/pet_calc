#include <QApplication>

#include "smartcalc.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  SmartCalc w;
  w.show();
  return a.exec();
}

/*
#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
*/
