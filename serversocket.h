#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;
class QTcpServer;

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    explicit ServerSocket(QObject *parent = nullptr);

private slots:
    void acceptConnection();
    void respond();
    void serverError(QAbstractSocket::SocketError);
    void socketError(QAbstractSocket::SocketError);

private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
};

#endif // SERVERSOCKET_H
