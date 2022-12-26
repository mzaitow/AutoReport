#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "Russian");

  QApplication a(argc, argv);

#ifdef Q_OS_WIN64
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
#endif

  MainWindow w;
  w.show();

  return a.exec();
}
