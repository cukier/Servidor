#include "serversocket.h"

#include "clistmodel.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

ServerSocket::ServerSocket(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_socket(nullptr)
{
    connect(m_server, &QTcpServer::newConnection,
            this, &ServerSocket::acceptConnection);
    connect(m_server, &QTcpServer::acceptError,
            this, &ServerSocket::serverError);

    if(!m_server->listen(QHostAddress::Any, 53593)) {
        qDebug() << "<ServerSocket> Server could not start";
    } else {
        qDebug() << "<ServerSocket> Servidor criado com sucesso";
    }
}

void ServerSocket::acceptConnection()
{
    if (!m_server)
        return;

    qDebug() << "<ServerSocket> nova conexao";

    if (m_socket) {
        m_socket->disconnect();
        delete m_socket;
        m_socket = nullptr;
        qDebug() << "<ServerSocket> criado novo socket";
    }

    m_socket = m_server->nextPendingConnection();

    if (!m_socket)
        return;

    connect(m_socket, &QTcpSocket::readyRead,
            this, &ServerSocket::respond);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &ServerSocket::socketError);
}

void ServerSocket::respond()
{
    if (!m_socket)
        return;

    QString in = QString(m_socket->readAll());

    qDebug() << "<ServerSocket> req: " << in;

    if (in.size()) {
        QThread::sleep(1);
        m_socket->write("pong");
        qDebug() << "<ServerSocket> pong";
    }
}

void ServerSocket::serverError(QAbstractSocket::SocketError serverError)
{
    if (serverError == QTcpSocket::RemoteHostClosedError)
        return;

    qDebug() << "<ServerSocket> server error: " << serverError;

    m_server->close();
}

void ServerSocket::socketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "<ServerSocket> server error: " << socketError;
    m_socket->close();
}
