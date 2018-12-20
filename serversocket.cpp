#include "serversocket.h"

#include "clistmodel.h"

#include <QTcpServer>
#include <QTcpSocket>

const int ServerSocket::chunkSize = 20;

ServerSocket::ServerSocket(QList<Usuario> usuario,
                           CListModel *model,
                           QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_socket(nullptr)
    , m_model(model)
    , m_usuario(usuario)
    , currentRow(0)
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

    qDebug() << "<ServerSocket> server error: " << serverError;

    m_server->close();
}

void ServerSocket::socketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "<ServerSocket> server error: " << socketError;
    m_socket->close();
}

void ServerSocket::enviarModelo()
{
    if (!m_socket) {
        qDebug() << "<ServerSocket> nao existe socket";
        return;
    }

    QString str;
    int totalRows = m_model->rowCount();
    int fullRows = int(totalRows / chunkSize);
    int to = chunkSize;

    if (currentRow >= (fullRows * chunkSize)) {
        to = totalRows % chunkSize;
    }

    for (int r = currentRow; r < to; ++r) {
        for (int c = 0; c < 6; ++c) {
            QModelIndex index = m_model->index(r, 0);

            str += m_model->data(index, docentryRole + c).toString();

            if (c < 5) {
                str += ';';
            } else {
                str += '\n';
            }
        }
    }

    currentRow += chunkSize;
    enviarMensagem(str);
    qDebug() << "<ServerSocket> enviado " << currentRow << " de " << totalRows;
}

void ServerSocket::enviarMensagem(QString str)
{
    if (!m_socket)
        return;

    m_socket->write(str.toUtf8());
    m_socket->flush();
    m_socket->waitForBytesWritten(5000);
    qDebug() << "<ServerSocket> escrito " << str.length();
}
