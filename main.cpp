
#include <QApplication>
#include "src/ui/Mainscene.h"

#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mainscene w;
    w.show();
    return a.exec();
}