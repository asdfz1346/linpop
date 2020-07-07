
#include <loggin.h>
#include <common.h>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Loggin::getInstance()->show();

    int iRet = app.exec();

//    appExitEvent();
    return iRet;
}
