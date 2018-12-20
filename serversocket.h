#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;
class QTcpServer;
class CListModel;

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    struct Usuario {
        QString nome;
        QString senha;
    };

    explicit ServerSocket(QList<Usuario> usuario,
                          CListModel *model,
                          QObject *parent = nullptr);

private slots:
    void acceptConnection();
    void respond();
    void serverError(QAbstractSocket::SocketError);
    void socketError(QAbstractSocket::SocketError);

private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
    CListModel *m_model;
    QList<Usuario> m_usuario;
    int currentRow;

    const static int chunkSize;
    void enviarModelo();
    void enviarMensagem(QString str);
};

#endif // SERVERSOCKET_H
