#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QVector<int>>("QIntVector");
    qRegisterMetaType<TextEditor>("TextEditor");
    qRegisterMetaType<QList<TextEditor>>("TextEditors");

    MainWindow w;
    w.show();
    return a.exec();
}
