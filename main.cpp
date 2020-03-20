#include "mainwindow.h"
#include <QApplication>

#if QT_VERSION < 0x050000
#include <QTextCodec>
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/icon.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
