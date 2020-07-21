#include <QCoreApplication>
#include "allsim.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AllSim ctp;
    ctp.setNLOOP(10);
    //ctp.indepDets_1();
    ctp.indepDets_2();
    ctp.clustDets_1();
    //ctp.clustDets_2();
    ctp.clustDets_2a();
    return a.exec();
}
