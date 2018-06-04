#ifndef TCPCLIENT_H
#define TCPCLIENT_H

// Qt
#include <QtCore>
#include <QtNetwork>

// Applicaon
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT TCPClient : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit TCPClient(QObject *pParent=nullptr);

    //! Destructor
    virtual ~TCPClient();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Connect to host
    bool connectToHost(const QString &sHost);

    //! Disconnect from host
    void disconnectFromHost();

    //! Send message
    void sendMessage(const QString &sMessage);

    //! Is connected?
    bool isConnected() const;

private:
    //! Int to byte array
    static QByteArray intToByteArray(qint32 iInput);

    //! Array to int
    static qint32 arrayToInt(QByteArray ba);

private:
    //! Socket
    QTcpSocket *m_pSocket=nullptr;

    //! Expected data size
    qint32 m_iExpectedDataSize = 0;

    //! Buffer
    QByteArray *m_pBuffer = nullptr;

public slots:
    //! Ready read
    void onReadyRead();

    //! Send message
    void onSendMessage(const QString &sMessage);

signals:
    //! Data ready
    void dataReady(const QByteArray &ba);
};
}

#endif // TCPCLIENT_H
