#include <QCoreApplication>

#include "serversocket.h"
#include "clistmodel.h"
#include "table.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerSocket servidor(&a);

    return a.exec();
}
