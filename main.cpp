#include "kruskalapp.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KruskalApp w;
    w.show();
    return a.exec();
}
