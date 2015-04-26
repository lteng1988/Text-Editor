#include <QApplication>

#include "notepad.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    Notepad mainWin;

    mainWin.show();

    return app.exec();
}
