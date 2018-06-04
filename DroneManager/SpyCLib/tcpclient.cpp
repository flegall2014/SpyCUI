// Application
#include "tcpclient.h"
using namespace Core;
#define LOCAL_HOST "127.0.0.1"
#define DATA_SIZE 4
#define PORT 1024

//-------------------------------------------------------------------------------------------------

TCPClient::TCPClient(QObject *pParent) : QObject(pParent)
{
    m_pSocket = new QTcpSocket();
    m_pBuffer = new QByteArray;
    connect(m_pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead, Qt::DirectConnection);
}

//-------------------------------------------------------------------------------------------------

TCPClient::~TCPClient()
{
    delete m_pBuffer;
    m_pSocket->deleteLater();
}

//-------------------------------------------------------------------------------------------------

bool TCPClient::connectToHost(const QString &sHost)
{
    m_pSocket->connectToHost(sHost, PORT);
    return m_pSocket->waitForConnected();
}

//-------------------------------------------------------------------------------------------------

void TCPClient::disconnectFromHost()
{
    m_pSocket->disconnectFromHost();
}

//-------------------------------------------------------------------------------------------------

void TCPClient::sendMessage(const QString &sMessage)
{
    if (m_pSocket != nullptr)
    {
        QByteArray ba = sMessage.toLatin1();
        if (m_pSocket->state() == QAbstractSocket::ConnectedState)
        {
            m_pSocket->write(intToByteArray(ba.size()));
            m_pSocket->write(ba);
            m_pSocket->waitForBytesWritten();
        }
    }
}

//-------------------------------------------------------------------------------------------------

QByteArray TCPClient::intToByteArray(qint32 iInput)
{
    QByteArray ba;
    QDataStream data(&ba, QIODevice::ReadWrite);
    data << iInput;
    return ba;
}

//-------------------------------------------------------------------------------------------------

qint32 TCPClient::arrayToInt(QByteArray ba)
{
    qint32 itemp;
    QDataStream data(&ba, QIODevice::ReadWrite);
    data >> itemp;
    return itemp;
}

//-------------------------------------------------------------------------------------------------

void TCPClient::onReadyRead()
{
    while (m_pSocket->bytesAvailable() > 0)
    {
        m_pBuffer->append(m_pSocket->readAll());

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

//-------------------------------------------------------------------------------------------------

void TCPClient::onSendMessage(const QString &sMessage)
{
    sendMessage(sMessage);
}

//-------------------------------------------------------------------------------------------------

bool TCPClient::isConnected() const
{
    return m_pSocket->state() == QAbstractSocket::ConnectedState;
}
