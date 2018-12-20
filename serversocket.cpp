#include "serversocket.h"

#include "clistmodel.h"

#include <QTcpServer>
#include <QTcpSocket>

const int ServerSocket::chunkSize = 100;

ServerSocket::ServerSocket(QList<Usuario> usuario,
                           CListModel *model,
                           QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_soket(nullptr)
    , m_model(model)
    , m_usuario(usuario)
{
    connect(m_server, &QTcpServer::newConnection,
            this, &ServerSocket::acceptConnection);
    connect(m_server, &QTcpServer::acceptError,
            this, &ServerSocket::serverError);

    if(!m_server->listen(QHostAddress::Any, 53593)) {
        qDebug() << "<Server> Server could not start";
    }
}

void ServerSocket::acceptConnection()
{
    if (!m_server)
        return;

    if (m_soket) {
        m_soket->disconnect();
        delete m_soket;
        m_soket = nullptr;
    }

    m_soket = m_server->nextPendingConnection();

    if (!m_soket)
        return;

    connect(m_soket, &QTcpSocket::readyRead,
            this, &ServerSocket::respond);
    connect(m_soket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(soketError(QAbstractSocket::SocketError)));
}

void ServerSocket::respond()
{
    if (!m_soket)
        return;

    QString in = QString(m_soket->readAll());

    qDebug() << "<Servidor> req: " << in;

    if (in.contains("login;")) {
        QStringList aux = in.split(';');

        if (aux.size() == 3) {
            QString nome = aux.at(1);
            QString senha = aux.at(2);

            for (auto i : m_usuario) {
                if (i.nome == nome && i.senha == senha) {
                    enviarModelo();
                }
            }
        }
    }
}

void ServerSocket::serverError(QAbstractSocket::SocketError serverError)
{
    if (serverError == QTcpSocket::RemoteHostClosedError)
        return;

    qDebug() << "<Servidor> server error: " << serverError;

    m_server->close();
}

void ServerSocket::enviarModelo()
{
    if (!m_soket) {
        qDebug() << "<Servidor> nao existe socket";
        return;
    }

    QString str;
    int totalRows = m_model->rowCount();
    int fullRows = int(totalRows / chunkSize);
    int to = chunkSize;

    if (currentRow >= fullRows) {
        to = totalRows % chunkSize;
    }

    for (int r = currentRow; r < to; ++r) {
        for (int c = 0; c < 6; ++c) {
            QModelIndex index = m_model->index(r, c);

            str += m_model->data(index, docentryRole + c).toString();

            if (c < 5) {
                str += ';';
            } else {
                str += '\n';
            }
        }
    }

    qDebug() << "<ServerSocket> enviado de " << currentRow << ' ' << to;
    currentRow += chunkSize;
    enviarMensagem(str);
}

void ServerSocket::enviarMensagem(QString str)
{
    if (!m_soket)
        return;

    m_soket->write(str.toUtf8());
    m_soket->flush();
    m_soket->waitForBytesWritten(5000);
}
