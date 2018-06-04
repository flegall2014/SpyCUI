#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QtCore>
#include <QtNetwork>

// Application
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT TCPServer : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit TCPServer(QObject *pParent=nullptr);

    //! Destructor
    virtual ~TCPServer();

    //! Send message
    void sendMessage(const QString &sMessage);

private:
    //! Array to int
    static qint32 arrayToInt(QByteArray ba);

    //! Int to array
    static QByteArray intToByteArray(qint32 iInput);

private:
    //! Expected data size
    qint32 m_iExpectedDataSize = 0;

    //! Server
    QTcpServer *m_pServer = nullptr;

    //! Buffer
    QByteArray *m_pBuffer = nullptr;

    //! Client sockets
    QVector<QTcpSocket *> m_vClients;

private slots:
    //! Handle incoming client connection
    void onNewConnection();

    //! Handle incoming client disconnection
    void onDisconnected();

    //! Ready read
    void onReadyRead();

signals:
    //! New connection from ground station
    void newConnectionFromGroundStation();

    //! Data ready
    void dataReady(const QByteArray &ba);
};
}

#endif
