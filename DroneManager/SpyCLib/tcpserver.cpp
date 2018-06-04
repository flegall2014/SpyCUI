// Application
#include "tcpserver.h"
#define DATA_SIZE 4
#define PORT 1024
using namespace Core;

//-------------------------------------------------------------------------------------------------

TCPServer::TCPServer(QObject *parent) : QObject(parent)
{
    m_pServer = new QTcpServer(this);
    m_pBuffer = new QByteArray;
    connect(m_pServer, QTcpServer::newConnection, this, &TCPServer::onNewConnection, Qt::DirectConnection);
    qDebug() << "Listening:" << m_pServer->listen(QHostAddress::Any, PORT);
}

//-------------------------------------------------------------------------------------------------

TCPServer::~TCPServer()
{
    delete m_pBuffer;
    m_pServer->deleteLater();
}

//-------------------------------------------------------------------------------------------------

void TCPServer::sendMessage(const QString &sMessage)
{
    foreach (QTcpSocket *pClient, m_vClients)
    {
        if (pClient != nullptr)
        {
            // Make sure socket is connected
            if (pClient->state() == QAbstractSocket::ConnectedState)
            {
                // Get byte array
                QByteArray ba = sMessage.toLatin1();

                // Write size of data
                pClient->write(intToByteArray(ba.size()));

                // Write data
                pClient->write(ba);
                pClient->waitForBytesWritten();
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------

void TCPServer::onNewConnection()
{
    qDebug() << "*** NEW CONNECTION ***";

    while (m_pServer->hasPendingConnections())
    {
        QTcpSocket *pSocket = m_pServer->nextPendingConnection();
        m_vClients << pSocket;
        connect(pSocket, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead, Qt::DirectConnection);
        connect(pSocket, &QTcpSocket::disconnected, this, &TCPServer::onDisconnected, Qt::DirectConnection);
    }

    // A new connection from ground station was detected
    emit newConnectionFromGroundStation();
}

//-------------------------------------------------------------------------------------------------

void TCPServer::onDisconnected()
{
    QTcpSocket *pSocket = static_cast<QTcpSocket *>(sender());
    if (pSocket != nullptr)
    {
        m_vClients.removeAll(pSocket);
        pSocket->deleteLater();
    }
}

//-------------------------------------------------------------------------------------------------

void TCPServer::onReadyRead()
{
    // Retrieve sender socket
    QTcpSocket *pSocket = static_cast<QTcpSocket *>(sender());
    if (pSocket != nullptr)
    {
        while (pSocket->bytesAvailable() > 0)
        {
            m_pBuffer->append(pSocket->readAll());

            // While can process data, process it
            while ((m_iExpectedDataSize == 0 && m_pBuffer->size() >= DATA_SIZE) || (m_iExpectedDataSize > 0 && m_pBuffer->size() >= m_iExpectedDataSize))
            {
                // If size of data has received completely, then store it on our global variable
                if (m_iExpectedDataSize == 0 && m_pBuffer->size() >= DATA_SIZE)
                {
                    m_iExpectedDataSize = arrayToInt(m_pBuffer->mid(0, DATA_SIZE));
                    m_pBuffer->remove(0, DATA_SIZE);
                }
                // If data has received completely, then emit our SIGNAL with the data
                if (m_iExpectedDataSize > 0 && m_pBuffer->size() >= m_iExpectedDataSize)
                {
                    QByteArray baData = m_pBuffer->mid(0, m_iExpectedDataSize);
                    m_pBuffer->remove(0, m_iExpectedDataSize);
                    m_iExpectedDataSize = 0;
                    emit dataReady(baData);
                    m_pBuffer->clear();
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

qint32 TCPServer::arrayToInt(QByteArray ba)
{
    qint32 itemp;
    QDataStream data(&ba, QIODevice::ReadWrite);
    data >> itemp;
    return itemp;
}

//-------------------------------------------------------------------------------------------------

QByteArray TCPServer::intToByteArray(qint32 iInput)
{
    QByteArray ba;
    QDataStream data(&ba, QIODevice::ReadWrite);
    data << iInput;
    return ba;
}
