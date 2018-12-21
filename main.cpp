#include <QCoreApplication>

#include "serversocket.h"
#include "clistmodel.h"
#include "table.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CListModel model;
    int rows = 23;

    for (int i = 0; i < rows; ++i) {
        TableRow tabela;

        tabela.State = "State" + QString::number(i);
        tabela.StcCode = "StcCode" + QString::number(i);
        tabela.porcent = "porcent" + QString::number(i);
        tabela.ItemCode = "ItemCode" + QString::number(i);
        tabela.ItemName = "ItemName" + QString::number(i);
        tabela.docentry = "docentry" + QString::number(i);
        model.addData(tabela);
    }

    QList<ServerSocket::Usuario> usuarios;
    ServerSocket::Usuario mauricio;

    mauricio.nome = "mauricio";
    mauricio.senha = "mauricio1";
    usuarios.append(mauricio);

    ServerSocket servidor(usuarios, &model, &a);

    return a.exec();
}
